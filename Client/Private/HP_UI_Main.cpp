#include "stdafx.h"
#include "..\Public\HP_UI_Main.h"

CHP_UI_Main::CHP_UI_Main(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CHP_UI_Main::CHP_UI_Main(const CHP_UI_Main & rhs)
	: CUI(rhs)
{
}

HRESULT CHP_UI_Main::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CHP_UI_Main : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHP_UI_Main::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CHP_UI_Main : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CHP_UI_Main : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = 170;
	m_tUIInfo.fY = 95;
	m_tUIInfo.fSizeX = 200;
	m_tUIInfo.fSizeY = 50;

	return S_OK;
}

void CHP_UI_Main::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CHP_UI_Main::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CHP_UI_Main::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CHP_UI_Main : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHP_UI_Main::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HP_UI_Main"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CHP_UI_Main * CHP_UI_Main::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CHP_UI_Main*	pInstance = new CHP_UI_Main(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CHP_UI_Main"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHP_UI_Main::Clone(void * pArg)
{
	CHP_UI_Main*	pInstance = new CHP_UI_Main(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CHP_UI_Main"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP_UI_Main::Free()
{
	__super::Free();
}
