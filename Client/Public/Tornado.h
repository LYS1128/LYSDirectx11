#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Player.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CTornado final : public CGameObject
{
private:
	explicit CTornado(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CTornado(const CTornado& rhs);
	virtual ~CTornado() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


private:
	_float m_fTimeDeleta = 0.f;
	_float m_fPercent =1.f;
	_float m_fOriginPosY = 0.f;
	_vector m_fGO = XMVectorSet(0.f,0.f, 0.f, 0.f);
	_float m_fG = 0.f;
	_bool m_bTypeRed = false;

public:
	_bool Get_Red() { return m_bTypeRed; }

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	HRESULT Create_MaskTexture();
	

public:
	static CTornado* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END