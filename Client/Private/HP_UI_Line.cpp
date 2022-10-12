#include "stdafx.h"
#include "..\Public\HP_UI_Line.h"

CHP_UI_Line::CHP_UI_Line(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CHP_UI_Line::CHP_UI_Line(const CHP_UI_Line & rhs)
	: CUI(rhs)
{
}

HRESULT CHP_UI_Line::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CHP_UI_Line : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHP_UI_Line::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CHP_UI_Line : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CHP_UI_Line : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = ((_float3*)pArg)->x;
	m_tUIInfo.fY = ((_float3*)pArg)->y;
	m_tUIInfo.fSizeX = 25;
	m_tUIInfo.fSizeY = 25;

	return S_OK;
}

void CHP_UI_Line::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CHP_UI_Line::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CHP_UI_Line::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CHP_UI_Line : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHP_UI_Line::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HP_UI_Line"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CHP_UI_Line * CHP_UI_Line::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CHP_UI_Line*	pInstance = new CHP_UI_Line(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CHP_UI_Line"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHP_UI_Line::Clone(void * pArg)
{
	CHP_UI_Line*	pInstance = new CHP_UI_Line(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CHP_UI_Line"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP_UI_Line::Free()
{
	__super::Free();
}
