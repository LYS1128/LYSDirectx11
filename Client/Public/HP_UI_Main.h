#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CHP_UI_Main final : public CUI
{
private:
	explicit CHP_UI_Main(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CHP_UI_Main(const CHP_UI_Main& rhs);
	virtual ~CHP_UI_Main() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

public:
	static CHP_UI_Main* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END