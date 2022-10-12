#pragma once
#include "Client_Defines.h"
#include "UI.h"

#include "Level_Loading.h"

BEGIN(Client)

class CGamePlayB final : public CUI
{
private:
	explicit CGamePlayB(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CGamePlayB(const CGamePlayB& rhs);
	virtual ~CGamePlayB() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();
	_bool		Get_GameStart() { return m_bGameStart; }

private:
	RECT	m_tRect;
	_bool	m_bGameStart = false;
	_bool	m_bOpenLevel = false;

public:
	static CGamePlayB* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END