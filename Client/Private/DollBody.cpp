#include "stdafx.h"
#include "..\Public\DollBody.h"
#include "GameInstance.h"

CDollBody::CDollBody(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CDollBody::CDollBody(const CDollBody & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDollBody::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDollBody::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	_uint Rand = rand();
	_vector Pos = *((_vector*)pArg);
	m_fOriginPosY = XMVectorGetY(Pos) - 0.3f;
	m_fGO = XMVectorSet(cos(Rand), (rand() % 11) / 10.f, sin(Rand),0.f);
	m_fG = rand() % 3 + 3.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);

	_vector vLook = XMVectorSet((rand() % 11) / 10.f -0.5f, (rand() % 11) / 10.f - 0.5f, (rand() % 11) / 10.f - 0.5f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	return S_OK;
}

void CDollBody::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_fTimeDeleta += TimeDelta;
	if (m_fTimeDeleta >= 2)
		m_eState = STATE_DEAD;

	m_fPercent -= TimeDelta / 2.f;
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float X = XMVectorGetX(vPos) + XMVectorGetX(m_fGO*0.01f);
	_float Y = XMVectorGetY(vPos) + 0.1f *(0.5f - m_fTimeDeleta);
	_float Z = XMVectorGetZ(vPos) + XMVectorGetZ(m_fGO*0.01f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(X, Y, Z, 1.f));

}

void CDollBody::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//CCollider*		pTargetAABB = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB"));
	//if (nullptr == pTargetAABB)
	//	return;

	////CCollider*		pTargetOBB = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB"));
	////if (nullptr == pTargetOBB)
	////	return;

	////CCollider*		pTargetSphere = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_SPHERE"));
	////if (nullptr == pTargetSphere)
	////	return;
	//if (pTargetAABB->Get_Active()) {
	//	if (m_pAABBCom->Collsion_AABB(pTargetAABB)) {
	//		m_eState = STATE_DEAD;
	//		CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_Player")->front());
	//		if (pPlayer->Get_Tooth() < 99)
	//			pPlayer->Set_Tooth(pPlayer->Get_Tooth() + 1);
	//	}
	//}
		
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CDollBody::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 5)))
			return E_FAIL;
	}	


	return S_OK;
}

HRESULT CDollBody::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_DollBody"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	
	return S_OK;
}

HRESULT CDollBody::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_fPercent", &m_fPercent, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPositionFloat4(), sizeof(_float4))))
	//	return E_FAIL;

	///* For.LightDesc */
	//const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	//if (nullptr == pLightDesc)
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
	//	return E_FAIL;	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CDollBody * CDollBody::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CDollBody*	pInstance = new CDollBody(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CDollBody"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDollBody::Clone(void * pArg)
{
	CDollBody*	pInstance = new CDollBody(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CDollBody"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDollBody::Free()
{
	__super::Free();	
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
