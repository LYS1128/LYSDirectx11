#pragma once

#include "Base.h"

BEGIN(Engine)

class CCustomFont final : public CBase
{
private:
	CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCustomFont() = default;

public:
	HRESULT NativeConstruct(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pText, _float2 vPosition, _fvector vColor, float rotation = 0, _float2 const& Float2Zero = _float2(0.f, 0.f), float scale = 1.f);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

private:	
	SpriteBatch*			m_pBatch = nullptr;
	SpriteFont*				m_pFont = nullptr;

public:
	static CCustomFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontFilePath);
	virtual void Free() override;
};

END