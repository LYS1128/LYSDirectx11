#include "stdafx.h"
#include "..\Public\MonTrailEffect.h"
#include "GameInstance.h"

CMonTrailEffect::CMonTrailEffect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CMonTrailEffect::CMonTrailEffect(const CMonTrailEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonTrailEffect::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonTrailEffect::NativeConstruct(void * pArg)
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

void CMonTrailEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_CGWeapon")->size() == 0) {
		m_eState = STATE_DEAD;
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	_float4x4		Mat;
	XMStoreFloat4x4(&Mat, XMMatrixRotationX(XMConvertToRadians(90.0f))* XMMatrixRotationZ(XMConvertToRadians(-90.0f))*XMMatrixTranspose(XMLoadFloat4x4(&dynamic_cast<CCGWeapon*>(pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, TEXT("Layer_CGWeapon"))->front())->Get_WorldMat())* XMMatrixRotationY(XMConvertToRadians(-180.0f)) *XMMatrixScaling(100.f, 100.f,100.f)));

	

	RELEASE_INSTANCE(CGameInstance);

	m_pVIBufferCom->Update(&Mat);


}

void CMonTrailEffect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);

}

HRESULT CMonTrailEffect::Render()
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

HRESULT CMonTrailEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MonTrailEffect"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_MonTrail"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMonTrailEffect::SetUp_ConstantTable()
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

CMonTrailEffect * CMonTrailEffect::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CMonTrailEffect*	pInstance = new CMonTrailEffect(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CMonTrailEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonTrailEffect::Clone(void * pArg)
{
	CMonTrailEffect*	pInstance = new CMonTrailEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CMonTrailEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonTrailEffect::Free()
{
	__super::Free();	
	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
