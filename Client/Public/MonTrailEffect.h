#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "CGWeapon.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTexture;
class CVIBuffer_Trail;
END

BEGIN(Client)

class CMonTrailEffect final : public CGameObject
{
private:
	explicit CMonTrailEffect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CMonTrailEffect(const CMonTrailEffect& rhs);
	virtual ~CMonTrailEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:	
	CRenderer*						m_pRendererCom = nullptr;
	CShader*						m_pShaderCom = nullptr;	
	CTexture*						m_pTextureCom = nullptr;
	CVIBuffer_Trail*		m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	

public:
	static CMonTrailEffect* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END