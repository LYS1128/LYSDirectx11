#include "stdafx.h"
#include "..\Public\FadeInOut.h"

CFadeInOut::CFadeInOut(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CFadeInOut::CFadeInOut(const CFadeInOut & rhs)
	: CUI(rhs)
{
}

HRESULT CFadeInOut::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CFadeInOut : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CFadeInOut::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CFadeInOut : NativeConstruct");
		return E_FAIL;
	}

	_float2 f = *((_float2*)pArg);
	if (f.x == 0)
		m_bBlack = false;
	else
		m_bBlack = true;

	if (f.y == 0) {
		m_bFadeOut = false;
		m_iAlpha = 0.98f;
	}
	else {
		m_bFadeOut = true;
		m_iAlpha = 0.02f;
	}

	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CFadeInOut : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = g_iWinCX/2.f;
	m_tUIInfo.fY = g_iWinCY / 2.f;
	m_tUIInfo.fSizeX = g_iWinCX;
	m_tUIInfo.fSizeY = g_iWinCY;

	return S_OK;
}

void CFadeInOut::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	++m_iTickcount;

	if (m_bFadeOut) {
		if (m_iTickcount > 6) {
			m_iTickcount = 0;
			m_iAlpha += 0.04f;
		}
		if (m_iAlpha >= 1.f)
			m_iAlpha = 1.f;
	}
	else {
		if (m_iTickcount > 6) {
			m_iTickcount = 0;
			m_iAlpha -= 0.02f;
		}
		if (m_iAlpha <= 0.f)
			m_iAlpha = 0.f;
	}

}

void CFadeInOut::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CFadeInOut::Render()
{
	if (m_bRenderTrue)
	{
		if (nullptr == m_pShaderCom ||
			nullptr == m_pVIBufferCom)
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fPercent", &m_iAlpha, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;

		_float4x4	ViewMatrix;
		XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;

		_float4x4	ProjMatrixTP;
		XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", m_iSprite)))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Begin(2)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CFadeInOut::SetUp_Components()
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

	if (m_bBlack) {
		/* For.Com_Texture */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_BOSS, TEXT("Prototype_Component_Texture_Black"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else
	{
		/* For.Com_Texture */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_BOSS, TEXT("Prototype_Component_Texture_White"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	return S_OK;
}

CFadeInOut * CFadeInOut::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CFadeInOut*	pInstance = new CFadeInOut(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CFadeInOut"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFadeInOut::Clone(void * pArg)
{
	CFadeInOut*	pInstance = new CFadeInOut(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CFadeInOut"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFadeInOut::Free()
{
	__super::Free();
}
