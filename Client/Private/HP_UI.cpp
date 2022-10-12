#include "stdafx.h"
#include "..\Public\HP_UI.h"

CHP_UI::CHP_UI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CHP_UI::CHP_UI(const CHP_UI & rhs)
	: CUI(rhs)
{
}

HRESULT CHP_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CHP_UI : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHP_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CHP_UI : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CHP_UI : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	switch (*((_uint*)pArg))
	{
	case 0:
		m_tUIInfo.fX = 165.f;
		m_tUIInfo.fY = 115.f;
		break;
	case 1:
		m_tUIInfo.fX = 178.f;
		m_tUIInfo.fY = 80.f;
		break;
	case 2:
		m_tUIInfo.fX = 191.f;
		m_tUIInfo.fY = 115.f;
		break;
	case 3:
		m_tUIInfo.fX = 204.f;
		m_tUIInfo.fY = 80.f;
		break;
	case 4:
		m_tUIInfo.fX = 217.f;
		m_tUIInfo.fY = 115.f;
		break;
	case 5:
		m_tUIInfo.fX = 230.f;
		m_tUIInfo.fY = 80.f;
		break;
	case 6:
		m_tUIInfo.fX = 243.f;
		m_tUIInfo.fY = 115.f;
		break;
	case 7:
		m_tUIInfo.fX = 256.f;
		m_tUIInfo.fY = 80.f;
		break;
	}

	m_tUIInfo.fSizeX = 25;
	m_tUIInfo.fSizeY = 25;

	m_iSprite = 8;

	return S_OK;
}

void CHP_UI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CHP_UI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CHP_UI::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CHP_UI : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHP_UI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HP_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CHP_UI * CHP_UI::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CHP_UI*	pInstance = new CHP_UI(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CHP_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHP_UI::Clone(void * pArg)
{
	CHP_UI*	pInstance = new CHP_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CHP_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP_UI::Free()
{
	__super::Free();
}
