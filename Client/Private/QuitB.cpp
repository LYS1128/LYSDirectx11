#include "stdafx.h"
#include "..\Public\QuitB.h"

CQuitB::CQuitB(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CQuitB::CQuitB(const CQuitB & rhs)
	: CUI(rhs)
{
}

HRESULT CQuitB::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CQuitB : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CQuitB::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CQuitB : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CQuitB : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = g_iWinCX * 0.675f;
	m_tUIInfo.fY = g_iWinCY * 0.7f;
	m_tUIInfo.fSizeX = 200.f;
	m_tUIInfo.fSizeY = 100.f;

	m_tRect.left = LONG(m_tUIInfo.fX - m_tUIInfo.fSizeX * 0.5f);
	m_tRect.top = LONG(m_tUIInfo.fY - m_tUIInfo.fSizeY * 0.5f);
	m_tRect.right = LONG(m_tUIInfo.fX + m_tUIInfo.fSizeX * 0.5f);
	m_tRect.bottom = LONG(m_tUIInfo.fY + m_tUIInfo.fSizeY * 0.5f);

	return S_OK;
}

void CQuitB::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt)) {
		m_iSprite = 1;
		m_pTransformCom->Scaled(_float3(m_tUIInfo.fSizeX*1.2f, m_tUIInfo.fSizeY*1.2f, 1.2f));
	}
		
	else{
		m_iSprite = 0;
		m_pTransformCom->Scaled(_float3(m_tUIInfo.fSizeX, m_tUIInfo.fSizeY, 1.f));
	}

	if (g_iLevel == LEVEL_GAMEPLAY)
		m_eState = STATE_DEAD;
		
}

void CQuitB::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CQuitB::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CBackground : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CQuitB::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo_QuitB"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CQuitB * CQuitB::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CQuitB*	pInstance = new CQuitB(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CQuitB"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuitB::Clone(void * pArg)
{
	CQuitB*	pInstance = new CQuitB(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CQuitB"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuitB::Free()
{
	__super::Free();
}
