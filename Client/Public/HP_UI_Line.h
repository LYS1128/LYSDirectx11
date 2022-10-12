#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CHP_UI_Line final : public CUI
{
private:
	explicit CHP_UI_Line(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CHP_UI_Line(const CHP_UI_Line& rhs);
	virtual ~CHP_UI_Line() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

public:
	static CHP_UI_Line* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END