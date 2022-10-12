#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CStartButterfly final : public CGameObject
{
private:
	explicit CStartButterfly(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CStartButterfly(const CStartButterfly& rhs);
	virtual ~CStartButterfly() = default;

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
	_vector m_Go;
	CTransform* m_pPlayerTrans = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	

public:
	static CStartButterfly* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END