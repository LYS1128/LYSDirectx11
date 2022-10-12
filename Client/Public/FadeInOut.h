#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CFadeInOut final : public CUI
{
private:
	explicit CFadeInOut(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CFadeInOut(const CFadeInOut& rhs);
	virtual ~CFadeInOut() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

private:
	_bool m_bBlack = false;
	_bool m_bFadeOut = false;
	_uint					m_iTickcount = 0;
	_float					m_iAlpha = 0.f;

public:
	_float Get_Alpha() { return m_iAlpha; }

public:
	static CFadeInOut* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END