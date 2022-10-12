#include "stdafx.h"
#include "..\Public\Laser.h"
#include "GameInstance.h"

CLaser::CLaser(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CLaser::CLaser(const CLaser & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLaser::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLaser::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	m_iIndex = *((_uint*)pArg);
	_vector vStartPos;

	switch (m_iIndex)
	{
	case 0:
		vStartPos = XMVectorSet(-18.f, 22.f, 145.f, 1.f);
		break;
	case 1:
		vStartPos = XMVectorSet(-23.f, 22.f, 138.f, 1.f);
		break;
	case 2:
		vStartPos = XMVectorSet(23.f, 22.f, 138.f, 1.f);
		break;
	case 3:
		vStartPos = XMVectorSet(18.f, 22.f, 145.f, 1.f);
		break;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_vector vQueenPos = dynamic_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_BOSS, L"Layer_Queen", L"Com_Transform"))->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f,7.f,0.f,0.f);
	
	_float3 fStartPos;
	XMStoreFloat3(&fStartPos, vStartPos);
	_float3 fQueenPos;
	XMStoreFloat3(&fQueenPos, vQueenPos);
	_float Dist = sqrtf((fStartPos.x - fQueenPos.x)*(fStartPos.x - fQueenPos.x) + (fStartPos.y - fQueenPos.y)*(fStartPos.y - fQueenPos.y) + (fStartPos.z - fQueenPos.z)*(fStartPos.z - fQueenPos.z));
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,XMVectorSet((fStartPos.x + fQueenPos.x)/2.f, (fStartPos.y + fQueenPos.y) / 2.f, (fStartPos.z + fQueenPos.z) / 2.f,1.f));
	

	_vector		vLook = vQueenPos - vStartPos;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 0.f, -1.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	m_pTransformCom->Scaled(_float3(0.5f, 0.5f, Dist / 2.f));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CLaser::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLaser::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
		
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
}

HRESULT CLaser::Render()
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
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 6)))
			return E_FAIL;
	}	


	return S_OK;
}

HRESULT CLaser::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_Laser"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	
	return S_OK;
}

HRESULT CLaser::SetUp_ConstantTable()
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

CLaser * CLaser::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLaser*	pInstance = new CLaser(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CLaser"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLaser::Clone(void * pArg)
{
	CLaser*	pInstance = new CLaser(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CLaser"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLaser::Free()
{
	__super::Free();	
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
