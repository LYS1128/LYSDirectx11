#include "stdafx.h"
#include "..\Public\HowToPlayB.h"

CHowToPlayB::CHowToPlayB(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CHowToPlayB::CHowToPlayB(const CHowToPlayB & rhs)
	: CUI(rhs)
{
}

HRESULT CHowToPlayB::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CHowToPlayB : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHowToPlayB::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CHowToPlayB : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CHowToPlayB : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = g_iWinCX * 0.71f;
	m_tUIInfo.fY = g_iWinCY * 0.5f;
	m_tUIInfo.fSizeX = 200.f;
	m_tUIInfo.fSizeY = 100.f;

	m_tRect.left = LONG(m_tUIInfo.fX - m_tUIInfo.fSizeX * 0.5f);
	m_tRect.top = LONG(m_tUIInfo.fY - m_tUIInfo.fSizeY * 0.5f);
	m_tRect.right = LONG(m_tUIInfo.fX + m_tUIInfo.fSizeX * 0.5f);
	m_tRect.bottom = LONG(m_tUIInfo.fY + m_tUIInfo.fSizeY * 0.5f);

	return S_OK;
}

void CHowToPlayB::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt)) {
		m_iSprite = 1;
		m_pTransformCom->Scaled(_float3(m_tUIInfo.fSizeX*1.2f, m_tUIInfo.fSizeY*1.2f, 1.2f));

		if (pGameInstance->Key_Down(VK_LBUTTON)) {
			if (m_bHTPUI == false) {
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("Layer_HowToPlay"), TEXT("Prototype_GameObject_HowToPlay")))) {
					MSG_BOX(TEXT("Failed to Created CLevel_Logo: Tick, Prototype_GameObject_GamePlayB"));
					return;
				}
				m_bHTPUI = true;
			}
			else {
				m_bHTPUI = false;
				dynamic_cast<CHowToPlay*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_HowToPlay")->front())->Close();
			}
		}
	}
	else{
		m_iSprite = 0;
		m_pTransformCom->Scaled(_float3(m_tUIInfo.fSizeX, m_tUIInfo.fSizeY, 1.f));
	}

	if (g_iLevel == LEVEL_GAMEPLAY)
		m_eState = STATE_DEAD;
	RELEASE_INSTANCE(CGameInstance);
}

void CHowToPlayB::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CHowToPlayB::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CBackground : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHowToPlayB::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo_HowToPlayB"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CHowToPlayB * CHowToPlayB::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CHowToPlayB*	pInstance = new CHowToPlayB(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CHowToPlayB"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHowToPlayB::Clone(void * pArg)
{
	CHowToPlayB*	pInstance = new CHowToPlayB(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CHowToPlayB"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHowToPlayB::Free()
{
	__super::Free();
}
