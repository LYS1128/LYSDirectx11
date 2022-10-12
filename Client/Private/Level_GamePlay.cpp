#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct())) {
		MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, super");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, Ready_Layer_BackGround");
		return E_FAIL;
	}
	if (FAILED(Ready_Lights())) {
		MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, Ready_Lights");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, Ready_Layer_Player");
		return E_FAIL;
	}
	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster")))) {
	//	MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, Ready_Layer_Monster");
	//	return E_FAIL;
	//}
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, Ready_Layer_Camera");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_SubUI(TEXT("Layer_UI")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, Ready_Layer_UI");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_UI(TEXT("Layer_HP_UI")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, Ready_Layer_HP_UI");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Object(TEXT("Layer_Object")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : NativeConstruct, Ready_Layer_BackGround");
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->StopSound(CSound_Manager::BGM);
	pGameInstance->StopSound(CSound_Manager::BIRD);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_iPreMode != g_iMode) {
		if (g_iMode == PMODE_SUPER) {
			pGameInstance->StopSound(CSound_Manager::BIRD);
			pGameInstance->StopSound(CSound_Manager::BGM);
			pGameInstance->SoundPlay(L"Hysteria_BGM.ogg", CSound_Manager::BIRD, SOUND_DEFAULT);
			m_iPreMode = g_iMode;
		}
		else if (g_iMode == PMODE_DEFAULT) {
			pGameInstance->StopSound(CSound_Manager::BIRD);
			pGameInstance->SoundPlay(L"LevelGamePlay_BGM.ogg", CSound_Manager::BGM, SOUND_SMALL);
			pGameInstance->SoundPlay(L"Bird.ogg", CSound_Manager::BIRD, SOUND_DEFAULT);
			m_iPreMode = g_iMode;
		}
	}
	else {
		if (g_iMode == PMODE_SUPER) {
			pGameInstance->SoundPlay(L"Hysteria_BGM.ogg", CSound_Manager::BIRD, SOUND_DEFAULT);
		}
		else if (g_iMode == PMODE_DEFAULT) {
			pGameInstance->SoundPlay(L"LevelGamePlay_BGM.ogg", CSound_Manager::BGM, SOUND_SMALL);
			pGameInstance->SoundPlay(L"Bird.ogg", CSound_Manager::BIRD, SOUND_DEFAULT);
		}
	}

	_uint size = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Effect")->size();
	CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());

	if (pPlayer->Get_CurrentIndex() == 238 && m_bMonsterSpawn1 == false) {
		m_bMonsterSpawn1 = true;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Monster", TEXT("Prototype_GameObject_Monster"),&XMVectorSet(75.391f,42.216f,144.460f,241.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
			return;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Monster", TEXT("Prototype_GameObject_Monster"),&XMVectorSet(81.064f, 42.353f, 133.458f, 249.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
			return;
		}

	}

	if (pGameInstance->Key_Pressing('O') && m_iGaraHP > 0)
		pPlayer->Set_Hp(pPlayer->Get_Hp() - 1);
	else if (pGameInstance->Key_Pressing('P') && m_iGaraHP < 79)
		pPlayer->Set_Hp(pPlayer->Get_Hp() + 1);

	m_iGaraHP = pPlayer->Get_Hp();
		//가짜HP조절하는부분

	//가짜HP로 장미 조절하는 부분
	_uint iRoseSize = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_HP_UI")->size();

	if (iRoseSize > 0)
	{
		auto iter = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_HP_UI")->begin();
		++iter;

		for (_uint i = 0; i < (m_iGaraHP / 10); ++i) {
			dynamic_cast<CUI*>(*iter)->Set_RenderTrue(true);
			dynamic_cast<CUI*>(*iter)->Set_Sprite(8);
			++iter;
		}

		if (m_iGaraHP % 10 != 0) {
			dynamic_cast<CUI*>(*iter)->Set_RenderTrue(true);
			dynamic_cast<CUI*>(*iter)->Set_Sprite(m_iGaraHP % 10 - 1);
		}
		else
			dynamic_cast<CUI*>(*iter)->Set_RenderTrue(false);

		for (_uint i = (m_iGaraHP / 10) + 2; i < iRoseSize; ++i) {
			++iter;
			dynamic_cast<CUI*>(*iter)->Set_RenderTrue(false);
		}
	}

	_uint iToothNum = pPlayer->Get_Tooth();
	auto iter = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_ToothNum")->begin();

	if (iToothNum < 10){
		dynamic_cast<CUI*>(*iter)->Set_Sprite(iToothNum % 10);
		++iter;
		dynamic_cast<CUI*>(*iter)->Set_Sprite(iToothNum / 10);
	}
	else {
		dynamic_cast<CUI*>(*iter)->Set_Sprite(iToothNum % 10);
		++iter;
		dynamic_cast<CUI*>(*iter)->Set_Sprite(iToothNum / 10);
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("게임 플레이 레벨"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 0.f);

	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Lights, Add_Lights");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 500.0f;
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Camera, Camera_Default");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Player")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
		return E_FAIL;
	}

	CBlade::SOCKETDESC			SocketDesc;
	ZeroMemory(&SocketDesc, sizeof(CBlade::SOCKETDESC));

	SocketDesc.pModelCom = (CModel*)pGameInstance->Get_Component(LEVEL_STATIC, pLayerTag, TEXT("Com_Model"));
	SocketDesc.pBoneName = "Bip01-Prop1";

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC,L"Layer_Weapon", TEXT("Prototype_GameObject_Blade"), &SocketDesc))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Weapon2", TEXT("Prototype_GameObject_Gun"), &SocketDesc))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_Trail"), &SocketDesc))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift2")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift3")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_ForkLift4")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Sky");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Cloud")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Water")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"),&XMVectorSet(3.f,5.f,0.f,1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(11.f, 3.f, 5.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(16.f, 10.f, 21.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-6.f, 6.f, 26.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(21.f, 15.f, 38.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-1.f, 20.f, 58.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(42.f, 19.f, 68.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(46.f, 16.f, 81.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(0.f, 20.f, 99.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(50.f, 36.f, 108.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(20.f, 40.f, 126.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(65.f, 49.f, 118.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(24.f, 53.f, 153.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(126.f, 53.f, 136.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(88.f, 54.f, 170.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(102.f, 31.f, 205.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(35.f, 31.f, 210.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(50.f, 24.f, 244.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(50.f, 24.f, 244.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(77.f, 3.f, 247.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(29.f, -8.f, 251.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(13.f, -5.f, 250.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-8.f, -4.f, 245.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-7.f, -4.f, 212.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-75.f, -23.f, 175.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-86.f, -27.f, 184.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-96.f, -24.f, 165.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-115.f, -19.f, 165.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-107.f, -9.f, 146.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Nutterfly"), &XMVectorSet(-133.f, -15.f, 154.f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
		return E_FAIL;
	}


	for (_uint i = 0; i <3; ++i) {
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(-16.979f, 79.586f, 274.413f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(17.995f, 32.172f, 28.451f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(5.445f, 46.020f, 25.146f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(58.700f, 72.932f, 222.637f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(-1.834f, 70.804f, 227.481f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(10.120f, 44.476f, 6.848f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(23.118f, 38.743f, -5.645f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(-16.505f, 77.764f, 248.974f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(16.236f, 45.480f, -3.269f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(41.677f, 74.249f, 84.250f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(69.731f, 60.308f, 241.338f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(6.392f, 21.138f, 28.114f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(-0.765f, 70.434f, 209.417f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(39.095f, 64.165f, 208.959f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(-3.536f, 25.410f, 24.757f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(39.409f, 52.465f, 119.179f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(30.100f, 66.491f, 58.663f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(15.533f, 41.758f, 22.258f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(0.982f, 11.077f, -1.115f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(5.990f, 27.258f, -0.227f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(-0.283f, 32.279f, -10.678f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(4.432f, 32.528f, 7.955f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(17.604f, 59.160f, 85.389f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(57.564f, 68.362f, 95.183f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(-8.702f, 54.813f, 48.038f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(-6.617f, 57.560f, 59.646f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(30.606f, 30.800f, 46.331f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(36.537f, 63.696f, 105.197f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(28.533f, 64.165f, 142.143f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Leave"), &XMVectorSet(61.656f, 76.212f, 268.743f, 1.f)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
			return E_FAIL;
		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Object(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	//for (_uint i = 0; i < 20; ++i)
	//{
	//	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tooth")))) {
	//		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
	//		return E_FAIL;
	//	}
	//}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_IronHive")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_PigNose")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Monster, PigNose");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Background", TEXT("Prototype_GameObject_JumpPad")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, JP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Background", TEXT("Prototype_GameObject_Flower"),&XMVectorSet(26.906f, -14.283, 253.360f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, JP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Background", TEXT("Prototype_GameObject_Flower"),&XMVectorSet(-119.283f,-15.565f,150.965f,1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, JP");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_CardGuard", TEXT("Prototype_GameObject_CardGuard")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Monster, IronHive");
		return E_FAIL;
	}

	CBlade::SOCKETDESC			SocketDesc;
	ZeroMemory(&SocketDesc, sizeof(CBlade::SOCKETDESC));

	SocketDesc.pModelCom = (CModel*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_CardGuard", TEXT("Com_Model"));
	SocketDesc.pBoneName = "Bip01-Prop1";

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_CGWeapon", TEXT("Prototype_GameObject_CardGuardWeapon"), &SocketDesc))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_MonTrail"), &SocketDesc))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"),&XMVectorSet(-6.621f,11.436f,68.612f,1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(-4.621f, 11.436f, 67.612f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(-6.621f, 11.436f, 66.612f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(30.908f, 11.393f, 83.494f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(28.908f, 11.393f, 85.494f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(31.908f, 11.393f, 86.494f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(35.477f, 30.284f,122.131f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(34.f, 30.284f, 121.131f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(-30.609f, -16.091f, 231.603f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(-34.190f, -17.000f, 231.764f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(-45.879f, -21.226f, 214.378f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(-61.373f, -23.254f, 206.466f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(-61.524f, -23.056f, 207.726f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Shell"), &XMVectorSet(-62.793f, -23.274f, 206.985f, 1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_IronHive")))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Monster, IronHive");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_CardGuard"),&XMVectorSet(0.f,0.f,0.f,1.f)))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Monster, IronHive");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_HP_UI_Main")))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_UI, HP_UI_Main"));
		return E_FAIL;
	}

	_uint iRoseNum = 0;
	for (_uint i = 0; i < 8; ++i)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_HP_UI"), &iRoseNum))) {
			MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_UI,HP_UI"));
			return E_FAIL;
		}
		++iRoseNum;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_SubUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Tooth_UI")))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,Tooth_UI"));
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Gun_UI")))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,Gun_UI"));
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Gun_Overload_UI")))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,Gun_Overload_UI"));
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Gun_Aim_UI")))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,Gun_Aim_UI"));
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Super_UI")))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,Gun_Aim_UI"));
		return E_FAIL;
	}

	_uint i = 0;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_ToothNum", TEXT("Prototype_GameObject_Tooth_Num_UI"),&i))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,Tooth_Num_UI"));
		return E_FAIL;
	}
	++i;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_ToothNum", TEXT("Prototype_GameObject_Tooth_Num_UI"),&i))) {
		MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,Tooth_Num_UI"));
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
