#include "stdafx.h"
#include "..\Public\Level_Logo.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct())) {
		MSG_BOX(TEXT("Failed ½Ø CLevel_Logo: NativeConstruct, super"));
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround")))) {
		MSG_BOX(TEXT("Failed ½Ø CLevel_Logo: NativeConstruct, Ready_Layer_BackGround"));
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->StopSound(CSound_Manager::BGM);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_Logo::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->SoundPlay(L"Logo_BGM.ogg", CSound_Manager::BGM, SOUND_DEFAULT);
	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CLevel_Logo : Render, super");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("·Î°í ·¹º§"));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);	

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_BackGround")))) {
		MSG_BOX(L"Failed To CLevel_Logo : Ready_Layer_BackGround, Prototype_GameObject_BackGround");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
