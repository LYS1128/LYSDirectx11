#include "stdafx.h"
#include "..\Public\Domino.h"
#include "GameInstance.h"

CDomino::CDomino(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CDomino::CDomino(const CDomino & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDomino::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDomino::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	
	m_vPos = *((_vector*)pArg);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0.f,10.f,0.f,0.f));
	return S_OK;
}

void CDomino::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	_float PosY = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	if (PosY >= XMVectorGetY(m_vPos)) {
		_vector Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos - XMVectorSet(0.f, 0.1f, 0.f, 0.f));
	}
	else
		m_bSpawn = true;

	if (m_bSpawn && m_bEnd == false) {
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
		pPlayer->Set_PlayerMode(0);
		m_bEnd = true;
		RELEASE_INSTANCE(CGameInstance);
	}
}

void CDomino::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

		
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}
	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CDomino::Render()
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

HRESULT CDomino::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Domino"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CDomino::SetUp_ConstantTable()
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

CDomino * CDomino::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CDomino*	pInstance = new CDomino(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CDomino"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDomino::Clone(void * pArg)
{
	CDomino*	pInstance = new CDomino(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CDomino"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDomino::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
