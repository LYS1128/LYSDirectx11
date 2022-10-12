#include "stdafx.h"
#include "..\Public\Tornado.h"
#include "GameInstance.h"

CTornado::CTornado(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CTornado::CTornado(const CTornado & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTornado::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTornado::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	m_bTypeRed = *((_bool*)pArg);

	

	if (FAILED(SetUp_Components()))
		return E_FAIL;	


	return S_OK;
}

void CTornado::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	//m_fPercent = 0.4;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pPlayerTrans->Get_State(CTransform::STATE_POSITION));
	//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta*5.0);


}

void CTornado::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

}

HRESULT CTornado::Render()
{



	return S_OK;
}

HRESULT CTornado::SetUp_Components()
{
	///* For.Com_Renderer */
	//if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
	//	return E_FAIL;

	///* For.Com_Shader */
	//if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
	//	return E_FAIL;

	//if (m_bTypeRed) {
	//	/* For.Com_Model */
	//	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_RedTornado"), (CComponent**)&m_pModelCom)))
	//		return E_FAIL;
	//}
	//else {
	//	/* For.Com_Model */
	//	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_WhiteTornado"), (CComponent**)&m_pModelCom)))
	//		return E_FAIL;
	//}

	//CCollider::COLLIDERDESC			ColliderDesc;
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//
	return S_OK;
}

HRESULT CTornado::SetUp_ConstantTable()
{
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//if (FAILED(m_pShaderCom->Set_RawValue("g_fPercent", &m_fPercent, sizeof(_float))))
	//	return E_FAIL;

	//if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	//
	//if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	//	return E_FAIL;	

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CTornado * CTornado::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CTornado*	pInstance = new CTornado(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTornado"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTornado::Clone(void * pArg)
{
	CTornado*	pInstance = new CTornado(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTornado"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTornado::Free()
{
	__super::Free();	
	//
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	//Safe_Release(m_pRendererCom);

}
