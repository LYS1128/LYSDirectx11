#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "BlendObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect.h"
#include "PIpeLine.h"
#include "Shader.h"
#include "Texture.h"

CRenderer::CRenderer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CComponent(pDeviceOut, pDeviceContextOut)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	_uint			iNumViewports = 1;

	D3D11_VIEWPORT	Viewport;

	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_LightDepth(m_pDevice, m_pDeviceContext)))
		return E_FAIL;

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Diffuse"), m_pDevice, m_pDeviceContext, Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Normal"), m_pDevice, m_pDeviceContext, Viewport.Width, Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Depth"), m_pDevice, m_pDeviceContext, Viewport.Width, Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Shade"), m_pDevice, m_pDeviceContext, Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Specular"), m_pDevice, m_pDeviceContext, Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	///* For.Target_Specular */
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_LightDepth"), m_pDevice, m_pDeviceContext, Viewport.Width, Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
	//	return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_AlphaBlend"), m_pDevice, m_pDeviceContext, Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	///* For.Target_Specular */
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_LightDepth"), m_pDevice, m_pDeviceContext, Viewport.Width, Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
	//	return E_FAIL;


	/* For.MRT_Deferred */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;

	/* For.MRT_LightAcc */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_LightDepth"))))
	//	return E_FAIL;

	/* For. MRT_AlphaBlend */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_AlphaBlend"), TEXT("Target_AlphaBlend"))))
		return E_FAIL;

	///* For. MRT_AlphaBlend */
	//if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_LightDepth"))))
	//	return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pTextureCom = CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Resource/Texture/Texture/Distortion.dds"));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = Viewport.Width;
	m_WorldMatrix._22 = Viewport.Height;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));

//
//#ifdef _DEBUG
	//if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(m_pDevice, m_pDeviceContext)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_LightDebugDesc(0.f, 0.f, 200.f, 200.f)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_LightDepth"), 0.f, 200.f, 200.f, 200.f)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Shade"), 0.f, 400.f, 200.f, 200.f)))
		//return E_FAIL;

//	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Diffuse"), 0.f, 0.f, 200.f, 200.f)))
//		return E_FAIL;
//	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Normal"), 0.f, 200.f, 200.f, 200.f)))
//		return E_FAIL;

//	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Specular"), 200.f, 200.f, 200.f, 200.f)))
//		return E_FAIL;
//	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_AlphaBlend"), 200.f, 400.f, 200.f, 200.f)))
////		return E_FAIL;
//	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_LightDepth"), 0.f, 0.f, 200.f, 200.f)))
//		return E_FAIL;


//#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Render_GameObjects(_uint Path)
{
	if (FAILED(Render_LightDepth()))
		return E_FAIL;

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_LightAcc()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_Final(Path)))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

//#ifdef _DEBUG
//
	//if (FAILED(Render_DebugComponents()))
	//	return E_FAIL;
//
//	m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"));
	//m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"));
//	m_pTarget_Manager->Render_Debug(TEXT("MRT_AlphaBlend"));
	//m_pTarget_Manager->Render_LightDebug();
//
//#endif // _DEBUG


	return S_OK;
}

HRESULT CRenderer::Add_DebugComponent(CComponent * pComponent)
{
	m_DebugComponents.push_back(pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	HRESULT hr = 0;

	for (auto& pGameObject : m_RenderList[GROUP_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_LightDepth()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Begin_MRT(m_pDeviceContext, TEXT("MRT_LightDepth"))))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_LightDepth(m_pDeviceContext)))
		return E_FAIL;

	HRESULT hr = 0;

	for (auto& pGameObject : m_RenderList[GROUP_LIGHTDEPTH])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render_LightDepth();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_LIGHTDEPTH].clear();

	//if (FAILED(m_pTarget_Manager->End_MRT(m_pDeviceContext, TEXT("MRT_LightDepth"))))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_LightDepth(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;

}

HRESULT CRenderer::Render_NonBlend()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pDeviceContext, TEXT("MRT_Deferred"))))
		return E_FAIL;

	HRESULT hr = 0;

	for (auto& pGameObject : m_RenderList[GROUP_NONBLEND])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_NONBLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pDeviceContext, TEXT("MRT_Deferred"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (nullptr == m_pTarget_Manager ||
		nullptr == m_pLight_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pDeviceContext, TEXT("MRT_LightAcc"))))
		return E_FAIL;

	m_pLight_Manager->Render_Lights();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pDeviceContext, TEXT("MRT_LightAcc"))))
		return E_FAIL;

	return S_OK;
}



HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderList[GROUP_NONLIGHT])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
				return E_FAIL;

			Safe_Release(pGameObject);
		}
	}
	m_RenderList[GROUP_NONLIGHT].clear();

	return S_OK;

}

HRESULT CRenderer::Render_Blend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pDeviceContext, TEXT("MRT_AlphaBlend"))))
		return E_FAIL;

	HRESULT hr = 0;

	m_RenderList[GROUP_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest)
	{
		return ((CBlendObject*)pSour)->Get_CamDistance() > ((CBlendObject*)pDest)->Get_CamDistance();
	});

	for (auto& pGameObject : m_RenderList[GROUP_ALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_ALPHABLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pDeviceContext, TEXT("MRT_AlphaBlend"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Final(_uint Path)
{
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_ShadeTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_SpecularTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_AlphaBlendTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_AlphaBlend")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_LightDepthTexture", m_pTarget_Manager->Get_LightDepthSRV())))
		return E_FAIL;
	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShader, "g_DistortionTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))));

	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);

	_vector vLightDir = XMVector4Normalize(XMVectorSet(1.f, -1.f, 1.f, 0.f));
	_vector vLightPos = -2.0f*180.0f*vLightDir + XMVectorSet(0.f, 0.f, 0.f, 1.f); // ¸ÊÀüÃ¼°¨½Î´Â¿ø¹ÝÁö¸§ 180
	_vector vTargetPos = XMVectorSet(-20.f, 26.f, 146.f, 1.f); //¸Ê Áß½ÉÁÂÇ¥
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_matrix V = XMMatrixLookAtLH(vLightPos, vTargetPos, vUp);

	_float3 SphereCenterLS;
	XMStoreFloat3(&SphereCenterLS, XMVector3TransformCoord(vTargetPos, V));

	_float l = SphereCenterLS.x - 180.f;
	_float b = SphereCenterLS.y - 180.f;
	_float n = SphereCenterLS.z - 180.f;
	_float r = SphereCenterLS.x + 180.f;
	_float t = SphereCenterLS.y + 180.f;
	_float f = SphereCenterLS.z + 180.f;

	_matrix P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	_float4x4 View;
	XMStoreFloat4x4(&View, XMMatrixTranspose(V));

	_float4x4 Proj;
	XMStoreFloat4x4(&Proj, XMMatrixTranspose(P));

	if (FAILED(m_pShader->Set_RawValue("g_matLightView", &View, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_matLightProj", &Proj, sizeof(_float4x4))))
		return E_FAIL;

	m_pShader->Begin(Path + 3);

	m_pVIBuffer->Render();

	return S_OK;

}

HRESULT CRenderer::Render_UI()
{
	HRESULT hr = 0;

	for (auto& pGameObject : m_RenderList[GROUP_UI])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_DebugComponents()
{
	for (auto& pComponent : m_DebugComponents)
	{
		if (nullptr != pComponent)
		{
			pComponent->Render();
			Safe_Release(pComponent);
		}
	}

	m_DebugComponents.clear();

	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CRenderer*	pInstance = new CRenderer(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < GROUP_END; ++i)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTextureCom);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

}


