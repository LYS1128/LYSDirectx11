#include "stdafx.h"
#include "..\Public\StrikeLine.h"
#include "GameInstance.h"

CStrikeLine::CStrikeLine(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CStrikeLine::CStrikeLine(const CStrikeLine & rhs)
	: CGameObject(rhs)
{
}

HRESULT CStrikeLine::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStrikeLine::NativeConstruct(void * pArg)
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

	_uint i = *((_uint*)pArg);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pPlayerTrans->Get_State(CTransform::STATE_POSITION) + pPlayerTrans->Get_State(CTransform::STATE_LOOK)+XMVectorSet(0.f,1.f,0.f,0.f));

	_vector		vLook = -1.f*pPlayerTrans->Get_State(CTransform::STATE_LOOK);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vUp =XMVectorSet(0.f, 1.f, 0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	_vector		vRight = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_UP));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	switch (i) {
	case 1:
		m_pTransformCom->Rotation(XMVector3Normalize(vLook), XMConvertToRadians(45.f));
			break;
	case 2:
		m_pTransformCom->Rotation(XMVector3Normalize(vLook), XMConvertToRadians(-45.f));
		break;
	case 3:
		m_pTransformCom->Rotation(XMVector3Normalize(vLook), XMConvertToRadians(45.f));
		break;
	}

	m_pTransformCom->Scaled(_float3(0.1f, 3.f, 1.f));

	return S_OK;
}

void CStrikeLine::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	++m_iTickcount;
	++m_iTime;

	if (m_iTime < 10) {
		if (m_iTickcount >= 1) {
			m_iTickcount = 0;
			m_iAlpha += 0.1f;
		}
	}
	else {
		if (m_iTickcount > 1) {
			m_iTickcount = 0;
			m_iAlpha -= 0.1f;
		}
	}

	if (m_iAlpha <= 0.f)
		m_eState = STATE_DEAD;
}

void CStrikeLine::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CStrikeLine::Render()
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

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CStrikeLine::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_StrikeLine"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStrikeLine::SetUp_ConstantTable()
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

CStrikeLine * CStrikeLine::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CStrikeLine*	pInstance = new CStrikeLine(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CStrikeLine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStrikeLine::Clone(void * pArg)
{
	CStrikeLine*	pInstance = new CStrikeLine(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CStrikeLine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStrikeLine::Free()
{
	__super::Free();	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
