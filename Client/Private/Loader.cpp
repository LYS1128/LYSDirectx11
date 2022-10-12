#include "stdafx.h"
#include "..\Public\Loader.h"

CLoader::CLoader(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: m_pDevice(pDeviceOut), m_pDeviceContext(pDeviceContextOut)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY ThreadEntryFunc(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_LevelIndex())
	{
	case LEVEL_GAMEPLAY:
		g_iLevel = LEVEL_GAMEPLAY;
		pLoader->Loading_ForGamePlayLevel();
		break;
	case LEVEL_BOSS:
		g_iLevel = LEVEL_BOSS;
		pLoader->Loading_ForBossLevel();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);

	if (0 == m_hThread) {
		MSG_BOX(L"Failed To CLoader : NativeConstruct, 0 == m_hThread");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HP_UI"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/HP_UI_%d.dds"), 9)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, HP_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HP_UI_Main"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/HP_UI_Main.dds"), 1)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, HP_UI_Main");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tooth_UI"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Tooth_UI.dds"), 1)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Tooth_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tooth_Num_UI"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Num_%d.dds"), 10)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Tooth_Num_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gun_UI"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Gun_UI_%d.dds"), 2)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Gun_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gun_Aim_UI"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Gun_Aim_UI.dds"), 1)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Gun_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gun_Overload_UI"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Gun_Overload_UI.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Gun_Overload_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RushBfly_G"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Bfly_0-crop%d.dds"), 13)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, RushBfly_G");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RushBfly_B"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Bfly_0R-crop%d.dds"), 13)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, RushBfly_B");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StartBfly_LB"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Bfly_1R-crop%d.dds"), 13)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_LB");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StartBfly_RB"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Bfly_1-crop%d.dds"), 13)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RB");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StartBfly_LP"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Bfly_2-crop%d.dds"), 13)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_LP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StartBfly_RP"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Bfly_2R-crop%d.dds"), 13)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Trail.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Super_UI"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/UI/Super.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Dissolve.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Spark"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Spark.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GlowLightRed"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Bfly_0-crop7.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GlowLight"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/GlowLight.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterWave"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/WaterWave.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DustEffect"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Dust.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MonTrailEffect"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Monster_Trail.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StartBfly_RP");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StrikeLine"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/StrikeLine.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, StrikeLine");
		return E_FAIL;
	}
	lstrcpy(m_szLoading, TEXT("모델을 로드중입니다. "));

	/*피벗 매트릭스*/
	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SK_Alice"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_AliceW/", "SK_AliceWZero2.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, SK_AliceW_Test");
		return E_FAIL;
	}

	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(90.0f))* XMMatrixRotationZ(XMConvertToRadians(-90.0f));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Blade"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/SK_VorpalBlade/", "SK_VorpalBlade.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Blade");
		return E_FAIL;
	}

	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(-90.0f))*XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Gun"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_PepperGrinderA/", "SK_PepperGrinderA.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Gun");
		return E_FAIL;
	}

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SK_Grunt_Mini"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_Grunt_Mini/", "SK_Grunt_MiniZero.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, SK_Grunt_Mini");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../../Map/", "Map0.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift2"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../../Map/", "Map7.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift3"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../../Map/", "Map8.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift4"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../../Map/", "Map9.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CarriageCar"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/CarriageCar/", "CarriageCar.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tooth"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/Tooth/", "Tooth.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Tooth");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(-90.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/Bullet/", "Bullet.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Bullet");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cloud"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/MapObj/MazeCloud_Mesh/", "MazeCloud_Mesh.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sky"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/MapObj/MazeSkyBox_Mesh/", "MazeSkyBox_Mesh.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Sky");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_IronHive"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/IronHive/", "IronHive.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, IronHive");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HivePartA"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/S_FX_BotlflyHive_Part02/", "S_FX_BotlflyHive_Part02.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, IronHive");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HivePartB"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/S_FX_BotlflyHive_Part04/", "S_FX_BotlflyHive_Part04.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, IronHive");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HivePartC"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/IronHive_Damage/", "IronHive_Damage.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, IronHive");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bolterfly"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_BolterflyB/", "SK_BolterflyB.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, SK_BolterflyB");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_JumpPad"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_GrowingJumpPad/", "SK_GrowingJumpPad.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, SK_BolterflyB");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Flower"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_ShrinkFlower/", "SK_ShrinkFlower.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, SK_BolterflyB");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PetalA"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/ShrinkFlower_Petal01/", "ShrinkFlower_Petal01.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PetalB"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/ShrinkFlower_Petal02/", "ShrinkFlower_Petal02.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PetalC"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/ShrinkFlower_Petal03/", "ShrinkFlower_Petal03.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PetalD"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/ShrinkFlower_Petal04/", "ShrinkFlower_Petal04.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Nutterfly"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_Nutterfly/", "SK_Nutterfly2.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, SK_BolterflyB");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Shell"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/BR_Tears/", "BR_Tears.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ShellPartA"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/BR_Tears_A/", "BR_Tears_A.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ShellPartB"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/BR_Tears_B/", "BR_Tears_B.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ShellPartC"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/BR_Tears_C/", "BR_Tears_C.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MetalPole"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/ScrapMetalPole/", "ScrapMetalPole.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FleshLamp"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/QFleshLampsC/", "QFleshLampsC.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_WindStroke"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/WindStroke/", "WindStroke.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, WindStroke");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_JumpFeatherR"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/WindStroke/", "JumpFeatherR.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_JumpFeatherG"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/WindStroke/", "JumpFeatherG.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_JumpFeatherB"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/WindStroke/", "JumpFeatherB.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeaveA"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/Leaves_Mesh01/", "Leaves_Mesh01.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeaveB"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/Leaves_Mesh02/", "Leaves_Mesh02.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PigNose"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_NoseSwitch_Fly/", "SK_NoseSwitch_Fly.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_WaterPlane"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/WaterPlane/", "WaterPlane.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CardGuard"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_CardGuard_Elite/", "SK_CardGuard_Elite2.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(-90.0f))* XMMatrixRotationZ(XMConvertToRadians(90.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CardGuardWeapon"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_CardGuard_Elite/", "CardGuardWeapenB.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-90.0f)) * XMMatrixRotationX(XMConvertToRadians(-90.0f))* XMMatrixRotationZ(XMConvertToRadians(90.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Domino"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/GiantToys_DominoA/", "GiantToys_DominoA.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	lstrcpy(m_szLoading, TEXT("셰이더를 로드중입니다. "));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"), CVIBuffer_Trail::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_Component, Prototype_Component_VIBuffer_Rect");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_MonTrail"), CVIBuffer_MonTrail::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_Component, Prototype_Component_VIBuffer_Rect");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_ExeTrail"), CVIBuffer_ExeTrail::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_Component, Prototype_Component_VIBuffer_Rect");
		return E_FAIL;
	}

	/* For. Prototype_Component_VIBuffer_RectInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance"), CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 30)))) {
		return E_FAIL;
	}

	/* For. Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, VtxNorTex");
		return E_FAIL;
	}
	/* For. Prototype_Component_Shader_VtxAnim*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"),CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnim.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, VtxAnim");
		return E_FAIL;
	}

	/* For. Prototype_Component_Shader_VtxAnim*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxBlur"), CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxBlur.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, VtxBlur");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstance"), CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxRectInstance.hlsl"), VTXINSTANCE_DECLARATION::Elements, VTXINSTANCE_DECLARATION::iNumElements)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, VtxBlur");
		return E_FAIL;
	}

	/* For. Prototype_Component_Shader_VtxNonAnim*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNonAnim.hlsl"), VTXNONANIMMODEL_DECLARATION::Elements, VTXNONANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;



	lstrcpy(m_szLoading, TEXT("콜라이더 생성중입니다. "));
	/* For.Prototype_Component_Collider_OBB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_AABB)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Collider_AABB");
		return E_FAIL;
	}

	lstrcpy(m_szLoading, TEXT("게임오브젝트를 로드중입니다. "));
	
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkLift"), CFork::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Fork-TestMap");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkLift2"), CForkTwo::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Fork-TestMap2");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkLift3"), CForkThree::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Fork-TestMap2");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkLift4"), CForkFour::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Fork-TestMap2");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tooth"), CTooth::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Tooth");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shell"), CShell::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShellPartA"), CShellPartA::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShellPartB"), CShellPartB::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShellPartC"), CShellPartC::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CarriageCar"), CCarriageCar::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Shell");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Blade"), CBlade::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Blade");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Trail"), CTrailEffect::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Blade");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gun"), CGun::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Gun");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet"), CBullet::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Bullet");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"), CSky::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Sky");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cloud"),CCloud::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Cloud");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Default"),CCamera_Default::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Camera_Default");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),CPlayer::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Player");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),CMonster::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Monster");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HP_UI"), CHP_UI::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, _HP_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HP_UI_Main"), CHP_UI_Main::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, HP_UI_Main");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tooth_UI"), CTooth_UI::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Tooth_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tooth_Num_UI"), CTooth_Num_UI::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Tooth_Num_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gun_UI"), CGun_UI::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Gun_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gun_Aim_UI"), CGun_Aim_UI::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Gun_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gun_Overload_UI"), CGun_Overload_UI::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, Gun_UI");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RushButterfly"), CRushButterfly::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, RushButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StartButterfly"), CStartButterfly::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindStroke"), CWindStroke::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FallStroke"), CFallStroke::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_JumpFeather"), CJumpFeather::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FallFeather"), CFallFeather::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Leave"), CLeave::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Flower"), CFlower::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Petal"), CPetal::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IronHive"), CIronHive::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HivePartA"), CHivePartA::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HivePartB"), CHivePartB::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HivePartC"), CHivePartC::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bolterfly"), CBolterfly::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_JumpPad"), CJumpPad::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Nutterfly"), CNutterfly::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, StartButterfly");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PigNose"), CPigNose::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, PigNose");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Domino"), CDomino::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, PigNose");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CardGuard"), CCardGuard::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, PigNose");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CardGuardWeapon"), CCGWeapon::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, PigNose");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MetalPole"), CMetalPole::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, PigNose");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FleshLamp"), CFleshLamp::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, PigNose");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WeaponLamp"), CWeaponLamp::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, PigNose");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Super_UI"), CSuper_UI::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, PigNose");
		return E_FAIL;
	}	
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GlowLight"), CGlowLight::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, GlowLight");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spark"), CSparkEffect::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, GlowLight");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Water"), CWaterPlane::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, GlowLight");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DustEffect"), CDustEffect::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Ready_Prototype_GameObject, GlowLight");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonTrail"), CMonTrailEffect::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StrikeLine"), CStrikeLine::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	lstrcpy(m_szLoading, TEXT("로딩 완료"));
	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForBossLevel()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_RedCircle"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Circle.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Circled");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_RedSquare"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Square.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Circled");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Black"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Black.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Circled");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_White"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/White.dds"))))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Circled");
		return E_FAIL;
	}
	lstrcpy(m_szLoading, TEXT("모델을 로드중입니다. "));

	/*피벗 매트릭스*/
	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_ForkBoss"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../../Map/", "Map11.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_NightSky"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/SkyBox/Skybox_Night/", "Skybox_Night.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_QueenBody"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_Queen/", "SK_Queen_Body.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_QueenBase"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_Queen/", "SK_Queen_Base.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_QueenMace"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_Queen/Queen_Mace/", "Queen_Mace.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_QueenTower"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_Queen/QueenTower/", "QueenTower.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Executioner"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_Executioner/", "SK_Executioner.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_ExeWeapon"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_Executioner/SK_ExeuctionerScythe/", "SK_ExeuctionerScythe.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Doll"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_DollGirl/", "SK_DollGirl.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_DollHead"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_DollGirl/", "SK_DollGirl_Head.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_DollLeft"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_DollGirl/", "SK_DollGirl_Left.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_DollRight"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_DollGirl/", "SK_DollGirl_Right.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_DollBody"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Dynamic/SK_DollGirl/", "SK_DollGirl_Body.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, JumpFeather");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Heart"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_DollGirl_Heart/", "SK_DollGirl_Heart.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_RedKnight"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/SlainKnight/", "SlainKnight_RottenRed.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_WhiteKnight"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/SlainKnight/", "SlainKnight_RottenWhite.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_RedTornado"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/TornadoMesh/", "TornadoRed.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_WhiteTornado"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/TornadoMesh/", "TornadoWhite.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Chandelier"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/QLightsH/", "QLightsH.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_HeartEye"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/QFleshLampsB/", "QFleshLampsB.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_Laser"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../../Resource/Mesh/Static/TowerLaser/", "TowerLaser.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Test_Map");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Model_SK_Alice_Queen"), CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../../Resource/Mesh/Dynamic/SK_Alice_Queen/", "Queen_Alice.fbx", PivotMatrix)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, SK_AliceW_Test");
		return E_FAIL;
	}

	lstrcpy(m_szLoading, TEXT("게임오브젝트를 로드중입니다. "));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ForkBoss"), CForkBoss::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Fork-TestMap2");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NightSky"), CNightSky::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForGamePlayLevel, Fork-TestMap2");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Queen"), CQueen::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QueenBase"), CQueenBase::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QueenTower"), CQueenTower::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Executioner"), CExecutioner::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExeWeapon"), CExeWeapon::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Doll"), CDoll::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DollHead"), CDollHead::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DollLeft"), CDollLeft::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DollRight"), CDollRight::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DollBody"), CDollBody::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Heart"), CHeart::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Knight"), CKnight::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tornado"), CTornado::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Chandelier"), CChandelier::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RedCircle"), CRedCircle::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RedSquare"), CRedSquare::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Laser"), CLaser::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QueenMace"), CQueenMace::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FadeInOut"), CFadeInOut::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QueenAlice"), CQueenAlice::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExeTrail"), CExeTrailEffect::Create(m_pDevice, m_pDeviceContext)))) {
		MSG_BOX(L"Failed To CMainApp : Loading_ForBoss, Queen");
		return E_FAIL;
	}

	lstrcpy(m_szLoading, TEXT("로딩 완료"));
	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);	
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
