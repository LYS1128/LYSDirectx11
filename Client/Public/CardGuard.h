#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Player.h"
#include "Blade.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CCardGuard final : public CGameObject
{
private:
	explicit CCardGuard(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CCardGuard(const CCardGuard& rhs);
	virtual ~CCardGuard() = default;

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
	CTexture*	m_pTextureCom = nullptr;

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
	_uint m_iHP = 1; //100
	_uint m_iDeadcount = 0;
	_bool m_bDead = false;
	_uint m_iCurrentCellIndex = 0;

public:
	_bool Get_CutSceneEnd() { return m_bCutSceneEnd; }

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	
	

public:
	static CCardGuard* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END