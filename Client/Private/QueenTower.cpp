#include "stdafx.h"
#include "..\Public\QueenTower.h"
#include "GameInstance.h"

CQueenTower::CQueenTower(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CQueenTower::CQueenTower(const CQueenTower & rhs)
	: CGameObject(rhs)
{
}

HRESULT CQueenTower::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQueenTower::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	
	_vector Arg = *((_vector*)pArg);
	m_iIndex = XMVectorGetW(Arg);




	m_vPos = XMVectorSetW(Arg, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0.f, 20.f + 20*m_iIndex, 0.f, 0.f));

	_float3 MyPos;
	XMStoreFloat3(&MyPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	_vector		vLook = XMVectorSet(0.f, 0.00, 130.82, 0.f) - XMVectorSet(MyPos.x, 0.f, MyPos.z, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CQueenTower::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	
	if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) >= XMVectorGetY(m_vPos)) {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) - XMVectorSet(0.f,1.f,0.f,0.f));
	}
	else if(m_bLaser == false)
	{
		m_bLaser = true;
		
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Laser", TEXT("Prototype_GameObject_Laser"), &m_iIndex))) {
			MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
			return;
		}

	}

	if (m_bDead) {
		if (m_bLaserDead == false) {
			m_bLaserDead = true;
			pGameInstance->SoundPlay(L"Tower_Destroy.ogg", CSound_Manager::SCENE_SOUND1, SOUND_DEFAULT); 
			pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Laser")->front()->Set_Dead();
		}

		++m_iDeadTick;

		m_pAttAABBCom->Set_Active(false);
		m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), TimeDelta*0.03f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) - XMVectorSet(0.f, 0.1f, 0.f, 0.f));

		if (m_iDeadTick > 5) {
			m_iDeadTick = 0;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Effect", TEXT("Prototype_GameObject_DustEffect"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
				return;
			}
		}

		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= -30.f)
			m_eState = STATE_DEAD;
	}

	m_pAttAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	RELEASE_INSTANCE(CGameInstance);
}

void CQueenTower::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

}

HRESULT CQueenTower::Render()
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
//	if (m_pAttAABBCom->Get_Active()) {
//		m_pAttAABBCom->Render();
//	}
//	//m_pOBBCom->Render();
//#endif // _DEBUG

	return S_OK;
}

HRESULT CQueenTower::Render_LightDepth()
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

HRESULT CQueenTower::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_QueenTower"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 12.5f, 0.f);
	ColliderDesc.vSize = _float3(5.f, 25.f, 5.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAttAABBCom, &ColliderDesc)))
		return E_FAIL;


	/* For.Com_OBB */
	//ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	//ColliderDesc.vSize = _float3(0.9f, 1.0f, 0.9f);

	//if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CQueenTower::SetUp_ConstantTable()
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



CQueenTower * CQueenTower::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CQueenTower*	pInstance = new CQueenTower(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CQueenTower"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQueenTower::Clone(void * pArg)
{
	CQueenTower*	pInstance = new CQueenTower(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CQueenTower"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQueenTower::Free()
{
	__super::Free();	
	//Safe_Release(m_pOBBCom);
	Safe_Release(m_pAttAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
