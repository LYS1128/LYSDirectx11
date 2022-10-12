#include "stdafx.h"
#include "..\Public\LoadGround.h"

CLoadGround::CLoadGround(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CLoadGround::CLoadGround(const CLoadGround & rhs)
	: CUI(rhs)
{
}

HRESULT CLoadGround::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CLoadGround : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoadGround::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CLoadGround : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CLoadGround : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = g_iWinCX * 0.5f;
	m_tUIInfo.fY = g_iWinCY * 0.5f;
	m_tUIInfo.fSizeX = g_iWinCX;
	m_tUIInfo.fSizeY = g_iWinCY;

	return S_OK;
}

void CLoadGround::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLoadGround::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CLoadGround::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CLoadGround : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoadGround::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Background"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CLoadGround * CLoadGround::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLoadGround*	pInstance = new CLoadGround(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CLoadGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoadGround::Clone(void * pArg)
{
	CLoadGround*	pInstance = new CLoadGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CLoadGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadGround::Free()
{
	__super::Free();
}
