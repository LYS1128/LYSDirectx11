#include "stdafx.h"
#include "..\Public\ForkFour.h"
#include "GameInstance.h"

CForkFour::CForkFour(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CForkFour::CForkFour(const CForkFour & rhs)
	: CGameObject(rhs)
{
}

HRESULT CForkFour::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CForkFour::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	return S_OK;
}

void CForkFour::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CForkFour::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	//if (nullptr != m_pRendererCom)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CForkFour::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}	

	return S_OK;
}

HRESULT CForkFour::Render_LightDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	_vector vLightDir = XMVector4Normalize(XMVectorSet(1.f, -1.f, 1.f, 0.f));
	_vector vLightPos = -2.0f*180.0f*vLightDir + XMVectorSet(0.f, 0.f, 0.f, 1.f); // ����ü���δ¿������� 180
	_vector vTargetPos = XMVectorSet(-20.f, 26.f, 146.f, 1.f); //�� �߽���ǥ
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_matrix V = XMMatrixLookAtLH(vLightPos, vTargetPos, vUp);

	_float3 SphereCenterLS;
	XMStoreFloat3(&SphereCenterLS, XMVector3TransformCoord(vTargetPos, V));

	_float l = SphereCenterLS.x - 180.f;
	_float b = SphereCenterLS.y - 180.f;
	_float n = SphereCenterLS.z - 180.f;
	_float r = SphereCenterLS.x + 180.f;
	_float t = SphereCenterLS.y + 180.f;
	_float f = SphereCenterLS.z + 180.f;

	_matrix P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	_float4x4 View;
	XMStoreFloat4x4(&View, XMMatrixTranspose(V));

	_float4x4 Proj;
	XMStoreFloat4x4(&Proj, XMMatrixTranspose(P));

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &View, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &Proj, sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CForkFour::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift4"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CForkFour::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CForkFour * CForkFour::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CForkFour*	pInstance = new CForkFour(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CForkFour"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CForkFour::Clone(void * pArg)
{
	CForkFour*	pInstance = new CForkFour(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CForkFour"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CForkFour::Free()
{
	__super::Free();	
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
