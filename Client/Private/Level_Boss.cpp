#include "stdafx.h"
#include "..\Public\Level_Boss.h"

CLevel_Boss::CLevel_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Boss::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct())) {
		MSG_BOX(L"Failed To CLevel_Boss : NativeConstruct, super");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround")))) {
		MSG_BOX(L"Failed To CLevel_Boss : NativeConstruct, Ready_Layer_BackGround");
		return E_FAIL;
	}
	if (FAILED(Ready_Lights())) {
		MSG_BOX(L"Failed To CLevel_Boss : NativeConstruct, Ready_Lights");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player")))) {
		MSG_BOX(L"Failed To CLevel_Boss : NativeConstruct, Ready_Layer_Player");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera")))) {
		MSG_BOX(L"Failed To CLevel_Boss : NativeConstruct, Ready_Layer_Camera");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_UI(TEXT("Layer_HP_UI")))) {
		MSG_BOX(L"Failed To CLevel_Boss : NativeConstruct, Ready_Layer_HP_UI");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_SubUI(TEXT("Layer_UI")))) {
		MSG_BOX(L"Failed To CLevel_Boss : NativeConstruct, Ready_Layer_UI");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Object(TEXT("Layer_Object")))) {
		MSG_BOX(L"Failed To CLevel_Boss : NativeConstruct, Ready_Layer_BackGround");
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->StopSound(CSound_Manager::BGM);
	pGameInstance->StopSound(CSound_Manager::BIRD);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_Boss::Tick(_double TimeDelta)
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
			if((pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->size() != 0 && dynamic_cast<CQueen*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->front())->Get_SoundOn()))
				pGameInstance->SoundPlay(L"LevelBoss_BGM.ogg", CSound_Manager::BGM, SOUND_SMALL);
			pGameInstance->SoundPlay(L"LevelBoss_Wind.ogg", CSound_Manager::BIRD, SOUND_DEFAULT);
			m_iPreMode = g_iMode;
		}
	}
	else {
		if (g_iMode == PMODE_SUPER) {
			pGameInstance->SoundPlay(L"Hysteria_BGM.ogg", CSound_Manager::BIRD, SOUND_DEFAULT);
		}
		else if (g_iMode == PMODE_DEFAULT) {
			if ((pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->size() != 0 && dynamic_cast<CQueen*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->front())->Get_SoundOn()))
				pGameInstance->SoundPlay(L"LevelBoss_BGM.ogg", CSound_Manager::BGM, SOUND_SMALL);
			pGameInstance->SoundPlay(L"LevelBoss_Wind.ogg", CSound_Manager::BIRD, SOUND_DEFAULT);
		}
	}

	CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());

	if (pPlayer->Get_CurrentIndex() == 10 && m_bMonsterSpawn1 == false) {
		m_bMonsterSpawn1 = true;
		pPlayer->Set_PlayerMode(4);
		pPlayer->Set_Scene(5);
	}
	if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->size() != 0) {
		CQueen* pQueen = (CQueen*)(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->front());
		if (pQueen->Get_AnimState() >= 3 && pQueen->Get_AnimState() < 6) {
			m_TimeAcc += TimeDelta;

			if (m_iPlayTime <= (_uint)m_TimeAcc) {
				m_TimeAcc = 0.0;
			}
		}
		else if (pQueen->Get_AnimState() == 6) {
			m_FourPhase += TimeDelta;
		}
	}
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

HRESULT CLevel_Boss::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("게임 플레이 레벨"));
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->size() != 0) {
		CQueen* pQueen = (CQueen*)(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->front());
		if (pQueen->Get_AnimState() >= 3) {
			wsprintf(m_szTime, TEXT("남은 시간 : %d"), m_iPlayTime - (_uint)m_TimeAcc);
			if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_MS"), m_szTime, _float2(90.f, 250.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0, _float2(0.f, 0.f), 0.3f)))
				return E_FAIL;
		}
		if (pQueen->Get_AnimState() == 3) {
			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Executioner") != nullptr && pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Executioner")->size() == 0)
				pQueen->Set_AnimState(4);
			wsprintf(m_szQuest, TEXT("몬스터를 처치하라"), m_iCount); //세미보스급 몬스터 소환
			if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_MS"), m_szQuest, _float2(90.f, 270.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0, _float2(0.f, 0.f), 0.25f)))
				return E_FAIL;
		}
		else if (pQueen->Get_AnimState() == 4) {

			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Doll") != nullptr)
				m_iCount = 4 - pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Doll")->size();

			if (m_iCount == 4)
				pQueen->Set_AnimState(5);

			wsprintf(m_szQuest, TEXT("인형들을 파괴하라 : %d / 4"), m_iCount); //자기 저격징 옮겨붙이기
			if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_MS"), m_szQuest, _float2(90.f, 270.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0, _float2(0.f, 0.f), 0.25f)))
				return E_FAIL;
		}
		else if (pQueen->Get_AnimState() == 5) {
			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Knight") != nullptr)
				m_iCount = 4 - pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Knight")->size();

			if (m_iCount == 4)
				pQueen->Set_AnimState(6);

			wsprintf(m_szQuest, TEXT("나이트를 파괴하라 : %d / 4"), m_iCount); //칼로만 때릴수 있고 속성 번갈아가면서 맞아야 데미지 안입는 장판 깔림
			if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_MS"), m_szQuest, _float2(90.f, 270.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0, _float2(0.f, 0.f), 0.25f)))
				return E_FAIL;
		}
		else if (pQueen->Get_AnimState() == 6) {

			if ((_uint)m_FourPhase == 30) {
				pQueen->Set_AnimState(7);
			}

			wsprintf(m_szQuest, TEXT("생존하라: %d / 30"), (_uint)m_FourPhase); //연속 줄무늬 장판(하나 지나고 나서 피하는거, 소인화해서 피하는거, 점프해서 피하는거 등등 그런데 이제 즉사기인)
			if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_MS"), m_szQuest, _float2(90.f, 270.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0, _float2(0.f, 0.f), 0.25f)))
				return E_FAIL;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Boss::Ready_Lights()
{

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Camera(const _tchar * pLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
	CTransform* pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));

	pPlayerTrans->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	pPlayer->Set_CurrentIndex(6);
	pPlayer->Set_PlayerMode(0);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, pLayerTag, TEXT("Prototype_GameObject_ForkBoss")))) {
		MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, pLayerTag, TEXT("Prototype_GameObject_NightSky")))) {
		MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, Sky");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Object(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Queen", TEXT("Prototype_GameObject_Queen")))) {
		MSG_BOX(L"Failed To CLevel_Boss : Boss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_QueenBase", TEXT("Prototype_GameObject_QueenBase")))) {
		MSG_BOX(L"Failed To CLevel_Boss : Boss, Queen");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_SubUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Boss * CLevel_Boss::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_Boss*	pInstance = new CLevel_Boss(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Boss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Boss::Free()
{
	__super::Free();
}
