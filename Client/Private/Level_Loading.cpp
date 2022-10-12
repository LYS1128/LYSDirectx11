#include "stdafx.h"
#include "..\Public\Level_Loading.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround")))) {
		MSG_BOX(L"Failed To CLevel_Logo : NativeConstruct, Ready_Layer_BackGround");
		return E_FAIL;
	}

	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_pLoader->Get_Finished())
	{
			CLevel*		pLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pDeviceContext);
				break;
			case LEVEL_BOSS:
				pLevel = CLevel_Boss::Create(m_pDevice, m_pDeviceContext);
				break;
			}

			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
				goto Finish;
	}

Finish:
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_tchar		szText[MAX_PATH] = TEXT("");

	lstrcpy(szText, TEXT("로딩 레벨입니다 : "));
	lstrcat(szText, m_pLoader->Get_LoadingText());

	SetWindowText(g_hWnd, szText);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_LoadGround")))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_BackGround"));
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_LoadKey")))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_BackGround"));
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);


}
