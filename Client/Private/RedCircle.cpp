#include "stdafx.h"
#include "..\Public\RedCircle.h"
#include "GameInstance.h"

CRedCircle::CRedCircle(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CRedCircle::CRedCircle(const CRedCircle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRedCircle::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRedCircle::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));

	RELEASE_INSTANCE(CGameInstance);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(rand()%79 - 39,0.5f, rand() % 92 + 43, 1.f));

	_vector vLook = XMVectorSet(0.f,1.f,0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(1.f, 0.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	m_pTransformCom->Scaled(_float3(20.f, 20.f, 1.f));

	return S_OK;
}

void CRedCircle::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	++m_iTickcount;

	m_pTransformCom->Turn(XMVectorSet(0.f,1.f,0.f,0.f),TimeDelta);

	if (m_iTickcount > 6) {
		m_iTickcount = 0;
		m_iAlpha -= 0.01f;
	}

	if (m_iAlpha <= 0.f) {
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Effect", TEXT("Prototype_GameObject_Chandelier"),&m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
			MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, Sky");
			return;
		}
		RELEASE_INSTANCE(CGameInstance);
		m_eState = STATE_DEAD;

	}

}

void CRedCircle::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CRedCircle::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(3);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CRedCircle::SetUp_Components()
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

		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_BOSS, TEXT("Prototype_Component_Texture_RedCircle"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	return S_OK;
}

HRESULT CRedCircle::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_fPercent", &m_iAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CRedCircle * CRedCircle::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CRedCircle*	pInstance = new CRedCircle(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CRedCircle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRedCircle::Clone(void * pArg)
{
	CRedCircle*	pInstance = new CRedCircle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CRedCircle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRedCircle::Free()
{
	__super::Free();	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
