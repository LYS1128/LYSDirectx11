#include "stdafx.h"
#include "..\Public\Sky.h"
#include "GameInstance.h"

CSky::CSky(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CSky::CSky(const CSky & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSky::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSky::NativeConstruct(void * pArg)
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

void CSky::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CSky::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	XMVECTOR vCamPos = pGameInstance->Get_CamPosition();

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos);

	RELEASE_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_PRIORITY, this);
}

HRESULT CSky::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player") != nullptr) {
		CPlayer* pPlayer = (CPlayer*)pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front();
		if (pPlayer->Get_Super()) {
			for (_uint i = 0; i < iNumMeshContainers; ++i)
			{
				if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
					return E_FAIL;


				if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 7)))
					return E_FAIL;
			}
		}
		else if (pPlayer->Get_PlayerMode() == 2) {
			for (_uint i = 0; i < iNumMeshContainers; ++i)
			{
				if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
					return E_FAIL;


				if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 8)))
					return E_FAIL;
			}
		}
		else {
			for (_uint i = 0; i < iNumMeshContainers; ++i)
			{
				if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
					return E_FAIL;


				if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 3)))
					return E_FAIL;
			}
		}
	}
	else
	{
		for (_uint i = 0; i < iNumMeshContainers; ++i)
		{
			if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
				return E_FAIL;


			if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 3)))
				return E_FAIL;
		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSky::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sky"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CSky::SetUp_ConstantTable()
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

CSky * CSky::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CSky*	pInstance = new CSky(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CSky"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSky::Clone(void * pArg)
{
	CSky*	pInstance = new CSky(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CSky"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky::Free()
{
	__super::Free();	
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}