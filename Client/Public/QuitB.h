#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CQuitB final : public CUI
{
private:
	explicit CQuitB(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CQuitB(const CQuitB& rhs);
	virtual ~CQuitB() = default;

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

public:
	static CQuitB* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END