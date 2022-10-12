#pragma once
#include "Client_Defines.h"
#include "UI.h"

#include "Player.h"

BEGIN(Client)

class CGun_Aim_UI final : public CUI
{
private:
	explicit CGun_Aim_UI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CGun_Aim_UI(const CGun_Aim_UI& rhs);
	virtual ~CGun_Aim_UI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

public:
	static CGun_Aim_UI* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END