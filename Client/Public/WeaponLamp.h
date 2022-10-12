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

class CWeaponLamp final : public CGameObject
{
private:
	explicit CWeaponLamp(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CWeaponLamp(const CWeaponLamp& rhs);
	virtual ~CWeaponLamp() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:	

	CPlayer*					m_pPlayer = nullptr;
	CTransform*				m_pPlayerTrans = nullptr;
	CCollider*			m_pAttAABBCom = nullptr;
	//CCollider*			m_pOBBCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	CModel*				m_pModelCom = nullptr;

private:
	_uint m_iAnimationIndex = 0;
	_vector m_vPlayerPos;
	_float Time = 0.f;
	_bool m_iSpawn = false;

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	
	

public:
	static CWeaponLamp* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END