#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;

	/* 장치초기화 등. 엔진의 기능을 활용하기위한 기초 초기화 작업을 마친다. */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext))) {
		MSG_BOX(L"Failed To CMainApp : NativeConstruct, Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Sounds()))
		return E_FAIL;
		
	if (FAILED(m_pGameInstance->Add_Fonts(m_pDevice, m_pDeviceContext, TEXT("Font_MS"), TEXT("../Bin/Resources/Fonts/Caffee24.spritefont"))))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component())) {
		MSG_BOX(L"Failed To CMainApp : NativeConstruct, Ready_Prototype_Component");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject())) {
		MSG_BOX(L"Failed To CMainApp : NativeConstruct, Ready_Prototype_GameObject");
		return E_FAIL;
	}

	if (FAILED(Open_Level(LEVEL_LOGO))) {
		MSG_BOX(L"Failed To CMainApp : NativeConstruct, Open_Level");
		return E_FAIL;
	}

	GetWindowRect(g_hWnd, &MouseRect);

	MouseRect.top += 10.f;
	MouseRect.left += 10.f;
	MouseRect.bottom -= 10.f;
	MouseRect.right -= 10.f;

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance) {
		MSG_BOX(L"Failed To CMainApp : Tick, nullptr == m_pGameInstance");
		return;
	}
	
	
	ClipCursor(&MouseRect);
	m_pGameInstance->Tick_Engine(TimeDelta);

#ifdef _DEBUG	
	m_TimeAcc += TimeDelta;
#endif // _DEBUG
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance) {
		MSG_BOX(L"Failed To CMainApp : Render, nullptr == m_pGameInstance");
		return E_FAIL;
	}

	m_pGameInstance->Render_Begin(_float4(0.0f, 0.f, 1.f, 1.f));


	if (g_iMode == PMODE_SUPER) {
		if (FAILED(m_pRenderer->Render_GameObjects(1))) {
			MSG_BOX(L"Failed To CMainApp : Render, Render_GameObjects");
			return E_FAIL;
		}
	}
	else if (g_iMode == PMODE_SMALL) {
		if (FAILED(m_pRenderer->Render_GameObjects(3))) {
			MSG_BOX(L"Failed To CMainApp : Render, Render_GameObjects");
			return E_FAIL;
		}
	}
	else {
		if (FAILED(m_pRenderer->Render_GameObjects(0))) {
			MSG_BOX(L"Failed To CMainApp : Render, Render_GameObjects");
			return E_FAIL;
		}
	}


	if (FAILED(m_pGameInstance->Render_Engine())) {
		MSG_BOX(L"Failed To CMainApp : Render, Render_Engine");
		return E_FAIL;
	}

//#ifdef _DEBUG
//	++m_iNumRender;
//
//	if (m_TimeAcc >= 1.0)
//	{
//		wsprintf(m_szFPS, TEXT("에프피에스 : %d"), m_iNumRender);
//		m_iNumRender = 0;
//		m_TimeAcc = 0.0;
//	}
//
//	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont, float rotation = 0, _float2 const& Float2Zero = _float2(0.f, 0.f), float scale = 1.f);
//	if (FAILED(m_pGameInstance->Render_Fonts(TEXT("Font_MS"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0, _float2(0.f, 0.f), 0.5f)))
//		return E_FAIL;
//#endif // _DEBUG

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelIndex)
{
	if (nullptr == m_pGameInstance) {
		MSG_BOX(L"Failed To CMainApp : Open_Level, nullptr == m_pGameInstance");
		return E_FAIL;
	}

	CLevel*			pLevel = nullptr;

	pLevel = CLevel_Logo::Create(m_pDevice, m_pDeviceContext);

	if (FAILED(m_pGameInstance->Open_Level(eLevelIndex, pLevel))) {
		MSG_BOX(L"Failed To CMainApp : Open_Level, Open_Level");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	/* 게임오브젝트: 첫 화면 UI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Prototype_GameObject_BackGround");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GamePlayB"), CGamePlayB::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Prototype_GameObject_GamePlayB");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HowToPlay"), CHowToPlay::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Prototype_GameObject_HowToPlayB");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HowToPlayB"), CHowToPlayB::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Prototype_GameObject_HowToPlayB");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuitB"), CQuitB::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Prototype_GameObject_QuitB");
		return E_FAIL;
	}

	/* 게임오브젝트: 로딩 화면 UI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadGround"), CLoadGround::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Prototype_GameObject_LoadGround");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadKey"), CLoadKey::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Prototype_GameObject_LoadKey");
		return E_FAIL;
	}

	/* 텍스쳐 Logo_Background */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo_Background"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Logo_Background_%d.dds"), 2)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, VIBuffer_Rect");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Background"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Loading_Background_%d.dds"), 1)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, VIBuffer_Rect");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo_GameStartB"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Logo_UI_GameStart_%d.dds"), 2)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, VIBuffer_Rect");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo_HowToPlayB"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Logo_UI_HowToPlay_%d.dds"), 2)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, VIBuffer_Rect");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo_QuitB"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Logo_UI_Quit_%d.dds"), 2)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, VIBuffer_Rect");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Key"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Loading_Key_%d.dds"), 1)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, VIBuffer_Rect");
		return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HowToPlay"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/HowToPlay.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, VIBuffer_Rect");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_Component, Prototype_Component_Renderer");
		return E_FAIL;
	}
	Safe_AddRef(m_pRenderer);

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_Component, Prototype_Component_Shader_VtxTex");
		return E_FAIL;
	}

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext)))){
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_Component, Prototype_Component_VIBuffer_Rect");
		return E_FAIL;
	}


	
	return S_OK;
}


CMainApp * Client::CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);	

	CGameInstance::Release_Engine();
}

