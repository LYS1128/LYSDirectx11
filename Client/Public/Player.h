#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Tornado.h"


BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	typedef struct tagSocketDesc
	{
		class CModel*	pModelCom = nullptr;
		const char*		pBoneName = nullptr;

	}SOCKETDESC;
public:
	enum PLAYERSTATE {STATE_IDLE,STATE_RUN,STATE_END};
private:
	explicit CPlayer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;

private:
	void WalkSound(_double TimeDelta);
	_double m_LFootStepTime = 0.0;
	_double m_RFootStepTime = 0.5;

	void VopalSound(_double TimeDelta);

	void HiccupSound(_double TimeDelta);
	_double m_HiccupTime = 0.0;

	_bool m_SlideSound = false;

	void DamageSound(_double TimeDelta);

	void GunFireSouund(_double TimeDelta);

public:
	void Key_Input(_double TimeDelta);

private:
	_uint m_iAnimationIndex = 37;
	_bool m_bMoving = false; //움직이는 중
	_bool m_bJumping = false;
	_float m_fJumpTime = 0.f;
	_uint m_iJumpNum = 0;
	_float m_fDropTime = 0.f;
	_float m_fAttachTime = 0.f;

	_uint m_iPlayerMode = 0; // 0: 칼모드 1: 총모드 2: 소인화모드 3. 슬라이드모드 4. 시네마모드(움직임x)
	_bool m_bDamaged = false;
	_uint m_iAttacking = 0;
	_bool m_bMosAttach = false;
	_bool m_bRush = false;
	_bool m_bDropDead = false;
	_float m_fDropDeadTime = 0.f;
	_bool m_bRespawn = false;
	_bool m_bSuperJump = false;
	_bool  m_bDest = false;
	_bool m_bDest2 = false;
	_bool m_bDest3 = false;
	_bool m_bFall = false;
	_uint m_iTooth = 0;
	_bool m_bSwardAABB = false;
	_double m_RushTime = 0.0;
	_bool m_bGunMode = false;
	_float m_fSlideTime = 1.f;
	_uint m_iScene = 0;
	_bool m_bSmall = false; //작아져있는 상태	
	_uint m_iCurrentCellIndex = 0;
	_float m_fAngle = 0.f;
	_bool m_bComboAttacking = false;
	_bool m_bTriAttacking = false;
	_uint m_iHp = 79;
	_float m_fGunfireTime = 0.f;
	_uint m_iGunTickCount = 0;
	_bool m_bFlower = false;
	_bool m_bSuper = false;
	_bool m_bSuperOn = false;
	_bool m_bSuperStart = false;
	_uint m_iShaderPath = 0;
public:
	void Set_ShaderPath(_uint i) { m_iShaderPath = i; }
	void Set_Super(_bool Super) { m_bSuper = Super; }
	void Set_Scene(_uint Num) { m_iScene = Num; }
	void Set_Hp(_uint Hp) { m_iHp = Hp; }
	void Set_Damaged(_int Damage) {
		if (Damage >= 0) {
			if (Damage <= m_iHp)
				m_iHp -= Damage;
			else
				m_iHp = 0;
		}
		else {
			if (-Damage <= 79-m_iHp)
				m_iHp -= Damage;
			else
				m_iHp = 0;
		}
			
	}
	void Set_Flower(_bool flower) { m_bFlower = flower; }
	void Set_Tooth(_uint Tooth) { m_iTooth = Tooth; }
	void Set_Jump(_bool Jump) { m_bJumping = Jump; }
	void Set_SuperJump(_bool Jump) { m_bSuperJump = Jump; }
	void Set_Attach(_bool Attach) { m_bMosAttach = Attach; }
	void Set_PlayerMode(_uint Mode) { m_iPlayerMode = Mode; }
	void Set_CurrentIndex(_uint Index) { m_iCurrentCellIndex = Index; }
	void Set_IsDamage(_bool DMG) { m_bDamaged = DMG; }
	_bool Get_Flower() { return m_bFlower; }
	_uint Get_SceneNum() { return m_iScene; }
	_bool Get_Rush() { return m_bRush; }
	_bool Get_DropDead() { return m_bDropDead; }
	_bool Get_Jump() { return m_bJumping; }
	_bool Get_GunMode() { return m_bGunMode; }
	_bool Get_Attach() { return m_bMosAttach; }
	_bool Get_Fall() { return m_bFall; }
	_uint Get_Hp() { return m_iHp; }
	_uint Get_Tooth() { return m_iTooth; }
	_bool Get_SwardAABB() { return m_bSwardAABB; }
	_uint Get_CurrentIndex() { return m_iCurrentCellIndex; }
	_uint Get_AnimationIndex() { return m_iAnimationIndex; }
	_uint Get_PlayerMode() { return m_iPlayerMode; }
	_float Get_GunTimePercent() { return m_fGunfireTime / 5.f; }
	_bool Get_Super() { return m_bSuper; }
	_bool Get_SuperOn() { return m_bSuperOn; }
	_bool Get_SuperStart() { return m_bSuperStart; }

private:	
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pAABBCom = nullptr;

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	PLAYERSTATE ePlayerState = STATE_IDLE;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	
	

public:
	static CPlayer* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END