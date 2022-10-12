#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CHowToPlay final : public CUI
{
private:
	explicit CHowToPlay(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CHowToPlay(const CHowToPlay& rhs);
	virtual ~CHowToPlay() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();
	void Close();

private:
	RECT	m_tRect;
	_bool m_bClose = false;

public:
	static CHowToPlay* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END