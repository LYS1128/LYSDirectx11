#pragma once
#include "Client_Defines.h"
#include "UI.h"

#include "HowToPlay.h"

BEGIN(Client)

class CHowToPlayB final : public CUI
{
private:
	explicit CHowToPlayB(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CHowToPlayB(const CHowToPlayB& rhs);
	virtual ~CHowToPlayB() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

private:
	RECT	m_tRect;
	_bool	m_bHTPUI = false;

public:
	static CHowToPlayB* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END