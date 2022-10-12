#include "stdafx.h"
#include "..\Public\Tooth_Num_UI.h"

CTooth_Num_UI::CTooth_Num_UI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CTooth_Num_UI::CTooth_Num_UI(const CTooth_Num_UI & rhs)
	: CUI(rhs)
{
}

HRESULT CTooth_Num_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CTooth_UI : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTooth_Num_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CTooth_UI : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CTooth_UI : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}
	switch (*((_uint*)pArg))
	{
	case 0:
		m_tUIInfo.fX = 125.f;
		m_tUIInfo.fY = 195.f;
		break;
	case 1:
		m_tUIInfo.fX = 115.f;
		m_tUIInfo.fY = 195.f;
		break;
	}

	m_tUIInfo.fSizeX = 10;
	m_tUIInfo.fSizeY = 10;

	return S_OK;
}

void CTooth_Num_UI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CTooth_Num_UI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CTooth_Num_UI::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CTooth_UI : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTooth_Num_UI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tooth_Num_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CTooth_Num_UI * CTooth_Num_UI::Create(ID3D11Device * pDeviceOut, ID3D11DeviceContext * pDeviceContextOut)
{
	CTooth_Num_UI*	pInstance = new CTooth_Num_UI(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTooth_Num_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTooth_Num_UI::Clone(void * pArg)
{
	CTooth_Num_UI*	pInstance = new CTooth_Num_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTooth_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTooth_Num_UI::Free()
{
	__super::Free();
}
