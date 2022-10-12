#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Client)

class CUI : public CGameObject
{
public:
	typedef struct tagUIInfo {
		_float fX;
		_float fY;
		_float fSizeX;
		_float fSizeY;
	}UIINFO;

protected:
	explicit CUI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

public:
	void Set_Sprite(_uint Num) { m_iSprite = Num; }
	void Set_RenderTrue(_bool Render) { m_bRenderTrue = Render; }
public:
	_bool	m_bRenderTrue = true;

protected:
	_float4x4			m_ProjMatrix;
	UIINFO				m_tUIInfo;
	_uint					m_iSprite = 0;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END