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

class CExecutioner final : public CGameObject
{
private:
	explicit CExecutioner(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CExecutioner(const CExecutioner& rhs);
	virtual ~CExecutioner() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;

private:	

	CPlayer*					m_pPlayer = nullptr;
	CTransform*				m_pPlayerTrans = nullptr;
	CCollider*			m_pAABBCom = nullptr;
	CCollider*			m_pAttAABBCom = nullptr;
	//CCollider*			m_pOBBCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	CModel*				m_pModelCom = nullptr;

private:
	_uint m_iSpawnAnim = 15;
	_uint m_iAttacking = 0;
	_uint m_iAnimationIndex = 9;
	_float m_fRandomMoveTime = 0.f;
	_bool m_bRandomMove = false;
	_uint m_iRandomnum = rand();
	_bool m_bCutScene = false;
	_bool m_bCutSceneEnd = false;
	_bool m_bDamaged = false;
	_bool m_bSpawned = false;
	_uint m_iHP = 50; //50
	_uint m_iTime = 0;

	_uint m_iCurrentCellIndex = 0;

public:
	_bool Get_CutSceneEnd() { return m_bCutSceneEnd; }
	void HitSound();
private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	
	

public:
	static CExecutioner* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END