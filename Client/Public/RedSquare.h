#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Player.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CRedSquare final : public CGameObject
{
private:
	explicit CRedSquare(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CRedSquare(const CRedSquare& rhs);
	virtual ~CRedSquare() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:	
	CRenderer*						m_pRendererCom = nullptr;
	CShader*						m_pShaderCom = nullptr;	
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	_uint					m_iSprite = 0;
	_uint					m_iTickcount = 0;
	_float					m_iAlpha = 1.f;
	_uint	m_iIndex;
	_float3 m_fSize;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	

public:
	static CRedSquare* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END