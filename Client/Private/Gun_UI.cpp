#include "stdafx.h"
#include "..\Public\Gun_UI.h"

CGun_UI::CGun_UI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CGun_UI::CGun_UI(const CGun_UI & rhs)
	: CUI(rhs)
{
}

HRESULT CGun_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CGun_UI : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CGun_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CGun_UI : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CGun_UI : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = 1100;
	m_tUIInfo.fY = 120;
	m_tUIInfo.fSizeX = 60;
	m_tUIInfo.fSizeY = 120;

	return S_OK;
}

void CGun_UI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CGun_UI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CGun_UI::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CGun_UI : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CGun_UI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gun_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CGun_UI * CGun_UI::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CGun_UI*	pInstance = new CGun_UI(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CGun_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGun_UI::Clone(void * pArg)
{
	CGun_UI*	pInstance = new CGun_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CGun_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGun_UI::Free()
{
	__super::Free();
}
