#include "stdafx.h"
#include "..\Public\Super_UI.h"

CSuper_UI::CSuper_UI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CSuper_UI::CSuper_UI(const CSuper_UI & rhs)
	: CUI(rhs)
{
}

HRESULT CSuper_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CSuper_UI : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSuper_UI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CSuper_UI : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CSuper_UI : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = 170;
	m_tUIInfo.fY = 95;
	m_tUIInfo.fSizeX = 200;
	m_tUIInfo.fSizeY = 50;

	return S_OK;
}

void CSuper_UI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (dynamic_cast<CPlayer*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front())->Get_Super() == true) {
		m_Time += TimeDelta;
		m_fPercent = (15.f - m_Time) / 15.f;

		if (m_Time >= 15.f) {
			dynamic_cast<CPlayer*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front())->Set_Super(false);
			m_Time = 0.f;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CSuper_UI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CSuper_UI::Render()
{
	if (m_bRenderTrue)
	{
		if (nullptr == m_pShaderCom ||
			nullptr == m_pVIBufferCom)
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fPercent", &m_fPercent, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;

		_float4x4	ViewMatrix;
		XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;

		_float4x4	ProjMatrixTP;
		XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", m_iSprite)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		if (dynamic_cast<CPlayer*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front())->Get_Super() == true)
		{
			if (FAILED(m_pVIBufferCom->Render()))
				return E_FAIL;
		}
		RELEASE_INSTANCE(CGameInstance);
	}

	return S_OK;
}

HRESULT CSuper_UI::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Super_UI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CSuper_UI * CSuper_UI::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CSuper_UI*	pInstance = new CSuper_UI(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CSuper_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSuper_UI::Clone(void * pArg)
{
	CSuper_UI*	pInstance = new CSuper_UI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CSuper_UI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSuper_UI::Free()
{
	__super::Free();
}
