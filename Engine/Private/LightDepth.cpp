#include "..\Public\LightDepth.h"
#include "Shader.h"

CLightDepth::CLightDepth(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CLightDepth::NativeConstruct()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = (_float)6000;
	m_Viewport.Height = (_float)6000;
	m_Viewport.MinDepth = 0.f;
	m_Viewport.MaxDepth = 1.f;

	ID3D11Texture2D*		pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 6000;
	TextureDesc.Height = 6000;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, &dsvDesc, &m_pDSV)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = TextureDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	if (FAILED(m_pDevice->CreateShaderResourceView(pDepthStencilTexture, &srvDesc, &m_pSRV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);
}

void CLightDepth::BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc)
{
	dc->RSSetViewports(1, &m_Viewport);
	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	dc->OMSetRenderTargets(1, renderTargets, m_pDSV);
	dc->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView * CLightDepth::Get_SRV() const
{
	return m_pSRV;
}

HRESULT CLightDepth::Ready_DebugDesc(_float fTopX, _float fTopY, _float fWidth, _float fHeight)
{
	m_fX = fTopX + fWidth * 0.5f;
	m_fY = fTopY + fHeight * 0.5f;
	m_fSizeX = fWidth;
	m_fSizeY = fHeight;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = m_fSizeX;
	m_WorldMatrix._22 = m_fSizeY;

	D3D11_VIEWPORT			Viewport;
	_uint					iNumViewport = 1;
	m_pDeviceContext->RSGetViewports(&iNumViewport, &Viewport);

	m_WorldMatrix._41 = m_fX - Viewport.Width * 0.5f;
	m_WorldMatrix._42 = -m_fY + Viewport.Height* 0.5f;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	return S_OK;
}

HRESULT CLightDepth::Bind_WorldMatrix(CShader * pShader)
{
	pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	pShader->Set_ShaderResourceView("g_Texture", m_pSRV);

	return S_OK;
}

CLightDepth * CLightDepth::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLightDepth*	pInstance = new CLightDepth(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed To Creating CRenderTarget"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightDepth::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
