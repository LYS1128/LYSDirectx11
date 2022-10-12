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

class CDoll final : public CGameObject
{
private:
	explicit CDoll(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CDoll(const CDoll& rhs);
	virtual ~CDoll() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;

private:	
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pAABBCom = nullptr;
private:
	_uint m_iAnimationIndex = 0;
	_float Time = 0.f;
	_uint Rand = 1;
	_uint m_iCurrentCellIndex = 0;
	_uint m_iHP = 1;
	_bool m_bTarget = false;
	_vector m_vPrePos;
	_uint m_iStuckTime = 0;
public:
	void Set_HP_Zero() { m_iHP = 0; }
	void Set_Target(_bool B) { m_bTarget = B; }
	_bool Get_Target() { return m_bTarget; }

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	
	

public:
	static CDoll* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END