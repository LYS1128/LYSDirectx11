#pragma once
#include "Client_Defines.h"
#include "UI.h"

#include "Player.h"

BEGIN(Client)

class CGun_Overload_UI final : public CUI
{
private:
	explicit CGun_Overload_UI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CGun_Overload_UI(const CGun_Overload_UI& rhs);
	virtual ~CGun_Overload_UI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

private:
	_float m_fOverloadPercent = 0.f;

public:
	static CGun_Overload_UI* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END