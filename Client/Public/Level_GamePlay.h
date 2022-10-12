#pragma once

#include "Client_Defines.h"

#include "GameInstance.h"
#include "Level.h"

#include "Camera_Default.h"
#include "UI.h"

#include "Blade.h"


BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	explicit CLevel_GamePlay(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLevel_GamePlay() = default;

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
	_uint m_iGaraHP = 79;
	_uint m_iPreMode = PMODE_DEFAULT;
	_bool m_bMonsterSpawn1 = false;
	_bool m_bMonsterSpawn2 = false;
public:
	static CLevel_GamePlay* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual void Free() override;
};

END