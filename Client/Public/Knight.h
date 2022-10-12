#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Player.h"
#include "Tornado.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CKnight final : public CGameObject
{
private:
	explicit CKnight(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CKnight(const CKnight& rhs);
	virtual ~CKnight() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;

private:	
	CCollider*			m_pAABBCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	CModel*				m_pModelCom = nullptr;

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	_vector m_vPos;
	_uint m_iHP = 30;
	_float Time = 0.f;
	_uint m_iIndex = 0;
	_bool m_bTypeRed = false;
	_bool m_bPlayerIN = false;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	void HitSound();
	HRESULT Create_MaskTexture();
	

public:
	static CKnight* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END