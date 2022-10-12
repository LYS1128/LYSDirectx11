#include "stdafx.h"
#include "..\Public\DustEffect.h"
#include "GameInstance.h"

CDustEffect::CDustEffect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CDustEffect::CDustEffect(const CDustEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDustEffect::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDustEffect::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	_vector vPos = *((_vector*)pArg);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos - XMVectorSet(0.f,XMVectorGetY(vPos),0.f,0.f)+XMVectorSet(((rand() % 21) - 10) / 5.f, (rand() % 11) / 10.f, ((rand() % 21) - 10) / 5.f, 0.f));
	m_pTransformCom->Scaled(_float3(5.f, 5.f, 5.f));
	return S_OK;
}

void CDustEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	++m_iTickcount;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pCamTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Camera", L"Com_Transform"));
	_vector S = XMVectorSet(m_pTransformCom->Get_Scale().x, m_pTransformCom->Get_Scale().y, m_pTransformCom->Get_Scale().z, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, -1.f *S * pCamTrans->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, -1.f *S * pCamTrans->Get_State(CTransform::STATE_LOOK));
	RELEASE_INSTANCE(CGameInstance);

	_float3 Scale = m_pTransformCom->Get_Scale();
	_float3 NewScale = _float3(Scale.x + (m_iTickcount)/1500.f, Scale.y + (m_iTickcount) / 1500.f, Scale.y + (m_iTickcount) / 1500.f);
	m_pTransformCom->Scaled(NewScale);

	m_iAlpha = 2.f - ((_float)m_iTickcount / 120.f);
	if (m_iTickcount >= 120) {
		m_eState = STATE_DEAD;
	}

}

void CDustEffect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CDustEffect::Render()
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

	m_pShaderCom->Begin(5);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CDustEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_DustEffect"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

	return S_OK;
}

HRESULT CDustEffect::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	_float Width = 64.f;
	_float Height = 64.f;
	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	if (FAILED(m_pShaderCom->Set_RawValue("g_Rand", &m_iRand, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fPercent", &m_iAlpha, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CDustEffect * CDustEffect::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CDustEffect*	pInstance = new CDustEffect(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CDustEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDustEffect::Clone(void * pArg)
{
	CDustEffect*	pInstance = new CDustEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CDustEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDustEffect::Free()
{
	__super::Free();	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
