#pragma once

#include "Client_Defines.h"

#include "GameInstance.h"
#include "Level.h"

#include "UI.h"

#include "CarriageCar.h"


BEGIN(Client)

class CLevel_Boss final : public CLevel
{
private:
	explicit CLevel_Boss(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLevel_Boss() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar * pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SubUI(const _tchar* pLayerTag);

private:
	_uint		m_iGaraHP = 79;
	_bool m_bMonsterSpawn1 = false;
	_bool m_bMonsterSpawn2 = false;
	_double m_TimeAcc = 0.0;
	_uint m_iCount = 0;
	_uint m_iPlayTime = 300;
	_double m_FourPhase = 0.0;
	_tchar					m_szTime[MAX_PATH] = TEXT("");
	_tchar					m_szQuest[MAX_PATH] = TEXT("");
	_uint m_iPreMode = PMODE_DEFAULT;
public:
	static CLevel_Boss* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual void Free() override;
};

END