#include "stdafx.h"
#include "..\Public\TrailEffect.h"
#include "GameInstance.h"

CTrailEffect::CTrailEffect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CTrailEffect::CTrailEffect(const CTrailEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTrailEffect::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrailEffect::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(-90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	return S_OK;
}

void CTrailEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	_float4x4		Mat;
	XMStoreFloat4x4(&Mat, XMMatrixRotationX(XMConvertToRadians(90.0f))* XMMatrixRotationZ(XMConvertToRadians(-90.0f))*XMMatrixTranspose(XMLoadFloat4x4(&dynamic_cast<CBlade*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, TEXT("Layer_Weapon"))->front())->Get_WorldMat())* XMMatrixRotationY(XMConvertToRadians(-180.0f)) *XMMatrixScaling(100.f, 100.f,100.f)));

	RELEASE_INSTANCE(CGameInstance);

	m_pVIBufferCom->Update(&Mat);

}

void CTrailEffect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
	if ((pPlayer->Get_Attach() == false && pPlayer->Get_PlayerMode() == 0 && pPlayer->Get_Rush() == false && pPlayer->Get_DropDead() == false && pPlayer->Get_Jump() == false) && pPlayer->Get_SuperStart() == false) {
		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CTrailEffect::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTrailEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CTrailEffect::SetUp_ConstantTable()
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

CTrailEffect * CTrailEffect::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CTrailEffect*	pInstance = new CTrailEffect(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTrailEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrailEffect::Clone(void * pArg)
{
	CTrailEffect*	pInstance = new CTrailEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTrailEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrailEffect::Free()
{
	__super::Free();	
	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
