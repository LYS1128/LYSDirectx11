#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "CardGuard.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CExeWeapon final : public CGameObject
{
public:
	typedef struct tagSocketDesc
	{
		class CModel*	pModelCom = nullptr;
		const char*		pBoneName = nullptr;

	}SOCKETDESC;
private:
	explicit CExeWeapon(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CExeWeapon(const CExeWeapon& rhs);
	virtual ~CExeWeapon() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:	
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pAABBCom = nullptr;

private:
	SOCKETDESC			m_SocketDesc;
	_float4x4*			m_pSocketMatrix;
	_float4x4			m_PivotMatrix;
	_float4x4			m_pWorldMat;
private:
	CCardGuard*					m_pPlayer = nullptr;
	CTransform*				m_pPlayerTrans = nullptr;
	_bool					m_bAABB = false;

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;
public:
	_float4x4 Get_WorldMat() { return m_pWorldMat; }

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	HRESULT Create_MaskTexture();
	

public:
	static CExeWeapon* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END