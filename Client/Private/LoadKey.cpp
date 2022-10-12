#include "stdafx.h"
#include "..\Public\LoadKey.h"

CLoadKey::CLoadKey(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CLoadKey::CLoadKey(const CLoadKey & rhs)
	: CUI(rhs)
{
}

HRESULT CLoadKey::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CLoadKey : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoadKey::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CLoadKey : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CLoadKey : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = g_iWinCX * 0.5f;
	m_tUIInfo.fY = g_iWinCY * 0.4f;
	m_tUIInfo.fSizeX = 50;
	m_tUIInfo.fSizeY = 200;

	m_pTransformCom->Scaled(_float3(m_tUIInfo.fSizeX, m_tUIInfo.fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tUIInfo.fX - g_iWinCX * 0.5f, -m_tUIInfo.fY + g_iWinCY * 0.5f, 0.f, 1.f));


	return S_OK;
}

void CLoadKey::Tick(_double TimeDelta)
{
	//__super::Tick(TimeDelta);	
	
	m_dTime += TimeDelta;

	if (m_dTime <= 1) {
		m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), 0.1*TimeDelta);
		m_pTransformCom->Go_Right(5 * TimeDelta);
	}
	else if (m_dTime > 1 && m_dTime <= 3) {
		m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), -0.1*TimeDelta);
		m_pTransformCom->Go_Right(-5 * TimeDelta);
	}
	else if (m_dTime > 3 && m_dTime <= 4) {
		m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), 0.1*TimeDelta);
		m_pTransformCom->Go_Right(5 * TimeDelta);
	}
	else if (m_dTime > 4)
		m_dTime = 0;

}

void CLoadKey::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CLoadKey::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CBackground : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoadKey::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Key"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CLoadKey * CLoadKey::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLoadKey*	pInstance = new CLoadKey(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CLoadKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoadKey::Clone(void * pArg)
{
	CLoadKey*	pInstance = new CLoadKey(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CLoadKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadKey::Free()
{
	__super::Free();
}
