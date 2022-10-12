#pragma once
#include "Client_Defines.h"
#include "UI.h"

#include "Player.h"

BEGIN(Client)

class CSuper_UI final : public CUI
{
private:
	explicit CSuper_UI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CSuper_UI(const CSuper_UI& rhs);
	virtual ~CSuper_UI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

private:
	_float m_fPercent = 0.f;
	_double m_Time = 0.0;

public:
	static CSuper_UI* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END