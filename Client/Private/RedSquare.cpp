#include "stdafx.h"
#include "..\Public\RedSquare.h"
#include "GameInstance.h"

CRedSquare::CRedSquare(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CRedSquare::CRedSquare(const CRedSquare & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRedSquare::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRedSquare::NativeConstruct(void * pArg)
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

	m_iIndex = *((_uint*)pArg);
	switch (m_iIndex)
	{
	case 0:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-25.f, 0.3f, 93.f, 1.f));
		m_fSize = _float3(51.f,140.f,1.f);
		break;
	case 1:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(25.f, 0.3f, 93.f, 1.f));
		m_fSize = _float3(51.f, 140.f, 1.f);
		break;
	case 2:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.3f, 46.3f, 1.f));
		m_fSize = _float3(102.f, 46.7f, 1.f);
		break;
	case 3:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.3f, 93.f, 1.f));
		m_fSize = _float3(102.f, 46.7f, 1.f);
		break;
	case 4:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.3f, 139.7f, 1.f));
		m_fSize = _float3(102.f, 46.7f, 1.f);
		break;
	}
	_vector vLook = XMVectorSet(0.f,1.f,0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 0.f, 1.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	m_pTransformCom->Scaled(m_fSize);

	return S_OK;
}

void CRedSquare::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	++m_iTickcount;

	if (m_iTickcount > 10) {
		m_iTickcount = 0;
		m_iAlpha -= 0.05f;
	}

	if (m_iAlpha <= 0.f) {
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		_vector MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector PlayerPos = dynamic_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

		if (XMVectorGetX(PlayerPos) >= XMVectorGetX(MyPos) - (m_fSize.x / 2.f) && XMVectorGetX(PlayerPos) <= XMVectorGetX(MyPos) + (m_fSize.x / 2.f)) {
			if (XMVectorGetZ(PlayerPos) >= XMVectorGetZ(MyPos) - (m_fSize.y / 2.f) && XMVectorGetZ(PlayerPos) <= XMVectorGetZ(MyPos) + (m_fSize.y / 2.f)) {
				CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
				
				if (pPlayer->Get_Super() == false) {
					pPlayer->Set_Damaged(10);
					pPlayer->Set_IsDamage(true);
				}
			}
		}

		RELEASE_INSTANCE(CGameInstance);
		m_eState = STATE_DEAD;

	}

}

void CRedSquare::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CRedSquare::Render()
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

HRESULT CRedSquare::SetUp_Components()
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

		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_BOSS, TEXT("Prototype_Component_Texture_RedSquare"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	return S_OK;
}

HRESULT CRedSquare::SetUp_ConstantTable()
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

CRedSquare * CRedSquare::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CRedSquare*	pInstance = new CRedSquare(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CRedSquare"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRedSquare::Clone(void * pArg)
{
	CRedSquare*	pInstance = new CRedSquare(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CRedSquare"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRedSquare::Free()
{
	__super::Free();	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
