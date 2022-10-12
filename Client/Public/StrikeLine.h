#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CStrikeLine final : public CGameObject
{
private:
	explicit CStrikeLine(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CStrikeLine(const CStrikeLine& rhs);
	virtual ~CStrikeLine() = default;

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
	_float					m_iAlpha = 0.1f;
	_uint					m_rand1 = rand();
	_uint					m_rand2 = rand();
	_uint					m_rand3 = rand();
	_uint					m_iTime = 0;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	

public:
	static CStrikeLine* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END