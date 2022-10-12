#pragma once

#include "Client_Defines.h"
#include "Camera.h"
#include "Player.h"
#include "Queen.h"

BEGIN(Client)

class CCamera_Default final : public CCamera
{
private:
	explicit CCamera_Default(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CCamera_Default(const CCamera_Default& rhs);
	virtual ~CCamera_Default() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	_float m_fRad = 3.f;
	_float m_fTheta =45.f;
	_float m_fPhi = -90.f;
	CPlayer* m_pPlayer = nullptr;
	_vector Go;
	_uint Time = 0;
	_uint Stop = 0;
	_uint Stop2 = 0;
	_uint Stop3 = 0;

public:
	static CCamera_Default* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END