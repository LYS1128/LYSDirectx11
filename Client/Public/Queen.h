#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Player.h"
#include "Blade.h"
#include "QueenTower.h"
#include "QueenBase.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CQueen final : public CGameObject
{
private:
	explicit CQueen(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CQueen(const CQueen& rhs);
	virtual ~CQueen() = default;

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
	CTexture* m_pTextureCom = nullptr;

public:
	void Set_AnimIndex(_uint Num) { m_iAnimationIndex = Num; }
	void Set_AnimState(_uint State) { m_iAnimState = State; }
	_uint Get_AnimState() { return m_iAnimState; }
	_bool Get_SoundOn() { return m_bSound; }

private:
	_uint m_iAttacking = 0;
	_uint m_iAnimationIndex = 0;
	_float m_fRandomMoveTime = 0.f;
	_bool m_bRandomMove = false;
	_uint m_iRandomnum = rand();
	_bool m_bDoll = false;
	_bool m_bKnight = false;
	_float m_fAttSoon = 0.f;
	_float m_iHp = 50;
	_float m_HeartSpawn = 0.f;
	_uint m_iCurrentCellIndex = 0;
	_uint m_iAnimState = 0;
	_float m_fChandTIme = 5.f;
	_float m_fSquareTime = 0.f;
	_bool m_bCircle = false;
	_bool m_bTower = false;
	_bool m_bDead = false;
	_uint m_iDeadcount = 0;
	_bool m_bSound = false;

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	
	

public:
	static CQueen* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END