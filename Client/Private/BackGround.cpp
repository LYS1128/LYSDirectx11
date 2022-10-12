#include "stdafx.h"
#include "..\Public\BackGround.h"

CBackGround::CBackGround(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: CUI(rhs)
{
}

HRESULT CBackGround::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CBackGround : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CBackGround::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CBackGround : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CBackGround : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = g_iWinCX * 0.5f;
	m_tUIInfo.fY = g_iWinCY * 0.5f;
	m_tUIInfo.fSizeX = g_iWinCX;
	m_tUIInfo.fSizeY = g_iWinCY;

	return S_OK;
}

void CBackGround::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Down(VK_RETURN) && m_bButton == false) {
		m_bButton = true;
		m_iSprite = 1;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_GamePlayB")))) {
			MSG_BOX(TEXT("Failed to Created CLevel_Logo: Tick, Prototype_GameObject_GamePlayB"));
			return;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_HowToPlayB")))) {
			MSG_BOX(TEXT("Failed to Created CLevel_Logo: Tick, Prototype_GameObject_HowToPlayB"));
			return;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_QuitB")))) {
			MSG_BOX(TEXT("Failed to Created CLevel_Logo: Tick, Prototype_GameObject_QuitB"));
			return;
		}
	}

	if (g_iLevel == LEVEL_GAMEPLAY)
		m_eState = STATE_DEAD;

	RELEASE_INSTANCE(CGameInstance);
}

void CBackGround::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CBackGround::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CBackground : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CBackGround::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo_Background"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CBackGround*	pInstance = new CBackGround(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround*	pInstance = new CBackGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
}
