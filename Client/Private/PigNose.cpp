#include "stdafx.h"
#include "..\Public\PigNose.h"
#include "GameInstance.h"

CPigNose::CPigNose(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CPigNose::CPigNose(const CPigNose & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPigNose::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPigNose::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
	m_pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));

	RELEASE_INSTANCE(CGameInstance);

	m_pModelCom->Set_PreAnimIndex(11);
	m_pModelCom->Set_AnimationIndex(11);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.0f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-110.976f, -28.360, 203.378f, 1.f));
	m_pAABBCom->Set_Active(true);
	m_pAttAABBCom->Set_Active(false);
	return S_OK;
}

void CPigNose::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (m_iHp >= 40) {
		m_iAnimationIndex = 11;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
	}
	else if (m_iHp < 40 && m_iHp >= 30) {
		m_iAnimationIndex = 12;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
	}
	else if (m_iHp < 30 && m_iHp >= 20) {
		m_iAnimationIndex = 13;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
	}
	else if (m_iHp < 20 && m_iHp >= 10) {
		m_iAnimationIndex = 14;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
	}
	else if (m_iHp < 10 && m_iHp >= 1) {
		m_iAnimationIndex = 15;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
	}
	else if (m_iHp == 0) {
		m_iAnimationIndex = 16;
		m_pModelCom->Set_Cut(true);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
		if (m_pModelCom->Get_Animation(16)->Get_KeyFrameENd()) {
			m_eState = STATE_DEAD;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Domino", TEXT("Prototype_GameObject_Domino"), &XMVectorSet(-108.411f,-24.998f,163.813f, 1.f)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Domino", TEXT("Prototype_GameObject_Domino"), &XMVectorSet(-112.079f, -20.124f, 157.640f, 1.f)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
				}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Domino", TEXT("Prototype_GameObject_Domino"), &XMVectorSet(-104.079f, -28.124f, 169.640f, 1.f)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			for (_uint i = 0; i < 6; ++i) {
				if (FAILED(pGameInstance->Set_CellOption(535 + i,0)))
					return;
			}
			m_pPlayer->Set_PlayerMode(4);
			m_pPlayer->Set_Scene(1);
		}
	}

	_matrix AABBMat = m_pTransformCom->Get_WorldMatrix();
	AABBMat.r[3] += 0.3* m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	m_pAttAABBCom->Update(AABBMat);

	m_pModelCom->Set_AnimationIndex(m_iAnimationIndex);
	m_pModelCom->Update(TimeDelta);
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	//m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	RELEASE_INSTANCE(CGameInstance);
}

void CPigNose::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	list<CGameObject*>* pBulletList = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Bullet");

	if (pBulletList != nullptr) {
		for (auto iter : *pBulletList) {
			if (m_iHp > 0 && m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				--m_iHp;
				}
			}
		}

	//if (nullptr != m_pRendererCom)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPigNose::Render()
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

//#ifdef _DEBUG
//	m_pAABBCom->Render();
//
//	if (m_pAttAABBCom->Get_Active() == true && m_pAABBCom != nullptr)
//		m_pAttAABBCom->Render();
//	//m_pOBBCom->Render();
//#endif // _DEBUG

	return S_OK;
}

HRESULT CPigNose::Render_LightDepth()
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

HRESULT CPigNose::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PigNose"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.8f, 0.f);
	ColliderDesc.vSize = _float3(1.4f, 1.6f, 1.4f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;



	if (FAILED(__super::SetUp_Components(TEXT("Com_AttAABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAttAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	//ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	//ColliderDesc.vSize = _float3(0.9f, 1.0f, 0.9f);

	//if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CPigNose::SetUp_ConstantTable()
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

	/* For.LightDesc */
	/*const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;	
*/
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPigNose * CPigNose::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CPigNose*	pInstance = new CPigNose(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPigNose"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPigNose::Clone(void * pArg)
{
	CPigNose*	pInstance = new CPigNose(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPigNose"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPigNose::Free()
{
	__super::Free();	
	//Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pAttAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
