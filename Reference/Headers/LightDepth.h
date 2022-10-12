#pragma once

#include "Base.h"

BEGIN(Engine)

class CLightDepth final : public CBase
{
public:
	CLightDepth(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLightDepth() = default;
public:
	HRESULT NativeConstruct();
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext * dc);
	ID3D11ShaderResourceView* Get_SRV() const;

	HRESULT Ready_DebugDesc(_float fTopX, _float fTopY, _float fWidth, _float fHeight);
	HRESULT Bind_WorldMatrix(class CShader * pShader);

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;

private:
	ID3D11ShaderResourceView*	m_pSRV = nullptr;
	ID3D11DepthStencilView*		m_pDSV = nullptr;
	D3D11_VIEWPORT				m_Viewport;

#ifdef _DEBUG
private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4		m_WorldMatrix;
#endif // _DEBUG

public:
	static CLightDepth* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;

};
END
