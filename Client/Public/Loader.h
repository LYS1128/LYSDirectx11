#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "GameInstance.h"
#include "Camera_Default.h"

#include "HP_UI.h"
#include "HP_UI_Main.h"
#include "Tooth_UI.h"
#include "Tooth_Num_UI.h"
#include "Gun_UI.h"
#include "Gun_Aim_UI.h"
#include "Gun_Overload_UI.h"
#include "RushButterfly.h"
#include "StartButterfly.h"
#include "WindStroke.h"
#include "JumpFeather.h"
#include "FallStroke.h"
#include "FallFeather.h"
#include "Super_UI.h"

#include "Player.h"
#include "Blade.h"
#include "TrailEffect.h"
#include "Gun.h"
#include "Bullet.h"
#include "Monster.h"
#include "IronHive.h"
#include "HivePartA.h"
#include "HivePartB.h"
#include "HivePartC.h"
#include "Bolterfly.h"
#include "PigNose.h"
#include "CardGuard.h"
#include "CGWeapon.h"
#include "MetalPole.h"
#include "FleshLamp.h"
#include "WeaponLamp.h"
#include "CarriageCar.h"
#include "GlowLight.h"
#include "SparkEffect.h"
#include "WaterPlane.h"
#include "DustEffect.h"
#include "MonTrailEffect.h"
#include "StrikeLine.h"
#include "ExeTrailEffect.h"

#include "Fork.h"
#include "ForkTwo.h"
#include "ForkThree.h"
#include "ForkFour.h"
#include "ForkBoss.h"
#include "JumpPad.h"
#include "Tooth.h"
#include "Shell.h"
#include "ShellPartA.h"
#include "ShellPartB.h"
#include "ShellPartC.h"
#include "Domino.h"
#include "Nutterfly.h"
#include "Leave.h"
#include "Flower.h"
#include "Petal.h"

#include "Sky.h"
#include "Cloud.h"
#include "NightSky.h"

#include "Queen.h"
#include "QueenBase.h"
#include "QueenTower.h"
#include "Executioner.h"
#include "ExeWeapon.h"
#include "Doll.h"
#include "DollLeft.h"
#include "DollRight.h"
#include "DollHead.h"
#include "DollBody.h"
#include "Heart.h"
#include "Knight.h"
#include "Tornado.h"
#include "Chandelier.h"
#include "RedCircle.h"
#include "RedSquare.h"
#include "Laser.h"
#include "QueenMace.h"
#include "FadeInOut.h"
#include "QueenAlice.h"

BEGIN(Client)
class CLoader final : public CBase
{
private:
	explicit CLoader(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CriticalSection() { return m_CriticalSection; }
	LEVEL Get_LevelIndex() { return m_eLevel; }
	_bool Get_Finished() const { return m_isFinished; }
	const _tchar* Get_LoadingText() const { return m_szLoading;  }

public:
	HRESULT NativeConstruct(LEVEL eLevel);
	HRESULT Loading_ForGamePlayLevel();
	HRESULT Loading_ForBossLevel();

private:
	LEVEL			m_eLevel = LEVEL_END;
	_bool			m_isFinished = false;
	_tchar		m_szLoading[MAX_PATH] = TEXT("");

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	HANDLE				m_hThread = 0;
	CRITICAL_SECTION	m_CriticalSection;

public:
	static CLoader* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eLevel);
	virtual void Free() override;
};
END