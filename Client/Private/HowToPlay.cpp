#include "stdafx.h"
#include "..\Public\HowToPlay.h"

CHowToPlay::CHowToPlay(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CUI(pDeviceOut, pDeviceContextOut)
{
}

CHowToPlay::CHowToPlay(const CHowToPlay & rhs)
	: CUI(rhs)
{
}

HRESULT CHowToPlay::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) {
		MSG_BOX(L"Failed To CHowToPlay : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHowToPlay::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg))) {
		MSG_BOX(L"Failed To CHowToPlay : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components())) {
		MSG_BOX(L"Failed To CHowToPlay : NativeConstruct, SetUp_Components");
		return E_FAIL;
	}

	m_tUIInfo.fX = -360.f;
	m_tUIInfo.fY = 360.f;
	m_tUIInfo.fSizeX = 720.f;
	m_tUIInfo.fSizeY = 720.f;

	m_tRect.left = LONG(m_tUIInfo.fX - m_tUIInfo.fSizeX * 0.5f);
	m_tRect.top = LONG(m_tUIInfo.fY - m_tUIInfo.fSizeY * 0.5f);
	m_tRect.right = LONG(m_tUIInfo.fX + m_tUIInfo.fSizeX * 0.5f);
	m_tRect.bottom = LONG(m_tUIInfo.fY + m_tUIInfo.fSizeY * 0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tUIInfo.fX - g_iWinCX * 0.5f, -m_tUIInfo.fY + g_iWinCY * 0.5f, 0.f, 1.f));

	return S_OK;
}

void CHowToPlay::Tick(_double TimeDelta)
{
	m_pTransformCom->Scaled(_float3(m_tUIInfo.fSizeX, m_tUIInfo.fSizeY, 1.f));



	if (m_bClose) {
		if (XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) >= -1280) {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) - XMVectorSet(15.f, 0.f, 0.f, 0.f));
		}
		else {
			m_eState = STATE_DEAD;
		}
	}
	else {
		if (XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= -400) {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + XMVectorSet(15.f, 0.f, 0.f, 0.f));
		}
	}

	if (g_iLevel == LEVEL_GAMEPLAY)
		m_eState = STATE_DEAD;
}

void CHowToPlay::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CHowToPlay::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CBackground : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CHowToPlay::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HowToPlay"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CHowToPlay::Close()
{
	m_bClose = true;
}

CHowToPlay * CHowToPlay::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CHowToPlay*	pInstance = new CHowToPlay(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CHowToPlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHowToPlay::Clone(void * pArg)
{
	CHowToPlay*	pInstance = new CHowToPlay(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CHowToPlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHowToPlay::Free()
{
	__super::Free();
}
