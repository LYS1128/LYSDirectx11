#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLoadKey final : public CUI
{
private:
	explicit CLoadKey(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CLoadKey(const CLoadKey& rhs);
	virtual ~CLoadKey() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

public:
	_double m_dTime = 0;

public:
	static CLoadKey* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END