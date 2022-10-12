#include "stdafx.h"
#include "..\Public\Leave.h"
#include "GameInstance.h"

CLeave::CLeave(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CLeave::CLeave(const CLeave & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLeave::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeave::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	m_vPos = *((_vector*)pArg);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);

	_vector vLook = XMVectorSet((rand() % 10) / 10.f, (rand() % 10) / 10.f, (rand() % 10) / 10.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	m_fGO = XMVectorSet((rand() % 21 - 10) / 10.f, -(rand() % 11) / 5.f, (rand() % 21 - 10) / 10.f,0.f);
	XMVector4Normalize(m_fGO);

	m_pTransformCom->Scaled(_float3(2.f, 2.f, 2.f));

	return S_OK;
}

void CLeave::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < -15.f) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);

		_vector vLook = XMVectorSet((rand() % 10) / 10.f, (rand() % 10) / 10.f, (rand() % 10) / 10.f, 0.f);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

		_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

		_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

		m_fGO = XMVectorSet((rand() % 21 - 10) / 10.f, -(rand() % 11) / 5.f, (rand() % 21 - 10) / 10.f, 0.f);
		XMVector4Normalize(m_fGO);
	}

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos += m_fGO * TimeDelta*((rand() % 11) / 10.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Turn(XMVectorSet((_float)(Rand % 5) - 2.f + 0.1f, (_float)(Rand2 % 5) - 2.f + 0.1f, (_float)(Rand3 % 5) - 2.f + 0.1f, 0.f), TimeDelta*2.0);
}

void CLeave::LateTick(_double TimeDelta)
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
	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
	{
			//m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CLeave::Render()
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
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}	


	return S_OK;
}

HRESULT CLeave::Render_LightDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

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

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &View, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &Proj, sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLeave::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	_uint Rand = rand() % 2;
	/* For.Com_Model */
	if (Rand == 0) {
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeaveA"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
	}
	else if (Rand == 1){
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeaveB"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
	}

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	
	return S_OK;
}

HRESULT CLeave::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
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

CLeave * CLeave::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLeave*	pInstance = new CLeave(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CJumpFeather"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLeave::Clone(void * pArg)
{
	CLeave*	pInstance = new CLeave(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CJumpFeather"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeave::Free()
{
	__super::Free();	
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
