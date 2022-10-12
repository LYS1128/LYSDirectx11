#include "stdafx.h"
#include "..\Public\Bolterfly.h"
#include "GameInstance.h"

CBolterfly::CBolterfly(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CBolterfly::CBolterfly(const CBolterfly & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBolterfly::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBolterfly::NativeConstruct(void * pArg)
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
	pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
	RELEASE_INSTANCE(CGameInstance);

	m_pModelCom->Set_PreAnimIndex(7);
	m_pModelCom->Set_AnimationIndex(7);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.0f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-85.202f+((rand()%11)/10.f)-0.5f, -30.332 + ((rand() % 11) / 10.f) - 0.5f, 163.758f + ((rand() % 11) / 10.f) - 0.5f, 1.f));
	m_pAABBCom->Set_Active(true);
	m_pAttAABBCom->Set_Active(false);
	return S_OK;
}

void CBolterfly::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->SoundPlay(L"Bolterfly_Fly.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_WEIGHT);
	_float3 PlayerPos;
	XMStoreFloat3(&PlayerPos, m_pPlayerTrans->Get_State(CTransform::STATE_POSITION));
	_float3 MyPos;
	XMStoreFloat3(&MyPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	_float Dist = sqrtf((PlayerPos.x - MyPos.x)*(PlayerPos.x - MyPos.x) + (PlayerPos.y - MyPos.y)*(PlayerPos.y - MyPos.y) + (PlayerPos.z - MyPos.z)*(PlayerPos.z - MyPos.z));

	if (m_bDamaged) {
		m_bDead = true;
		m_iDeadcount +=2;
		m_iAnimationIndex = 0;
		m_pModelCom->Set_Cut(true);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
		if (m_pModelCom->Get_Animation(0)->Get_KeyFrameENd()) {
			m_eState = STATE_DEAD;
			HitSound();
			pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
			pGameInstance->SoundPlay(L"Bolterfly_Death.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_WEIGHT);
		}
	}
	else {

		if (m_iAttacking != 1) {
			_vector Go = XMVectorSet(PlayerPos.x, PlayerPos.y + 0.7f, PlayerPos.z, 0.f) - XMVectorSet(MyPos.x, MyPos.y, MyPos.z, 0.f);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(Go));

			_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

			_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
			m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
		}

		if (m_fAttSoon >= 1.f && m_iAttacking != 2) {
			m_iAnimationIndex = 2;
			m_pModelCom->Set_Cut(false);
			m_pModelCom->Set_Speed(1.f);
			if (m_iAttacking == 0) {
				m_pPlayer->Set_Attach(true);
				m_iAttacking = 1;
			}
			if (m_pPlayer->Get_Attach() == false) {
				m_iAttacking = 2;
				m_fAttSoon = 0.f;
			}
		}
		if (m_iAttacking != 1 && Dist >= 1.f) {
			m_fAttSoon = 0.f;
			m_iAnimationIndex = 4;
			m_pModelCom->Set_Cut(false);
			m_pModelCom->Set_Speed(1.f);
			m_pTransformCom->Go_Straight(TimeDelta);
		}
		else if (m_iAttacking != 1 && Dist < 1.f) {
			m_fAttSoon += TimeDelta;
			m_iAnimationIndex = 8;
			m_pModelCom->Set_Cut(false);
			m_pModelCom->Set_Speed(1.f);
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

void CBolterfly::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	list<CGameObject*>* pObjectList = pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_Bolterfly");

	if (pObjectList != nullptr) {
		for (auto iter : *pObjectList) {
			if (iter == this)
				continue;

			if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				_float3 MySize = _float3(0.7f,0.7f, 0.7f);
				_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
				_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
				m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
			}
		}
	}

	CCollider*		pTargetAABB = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Weapon"), TEXT("Com_AABB"));
	if (nullptr == pTargetAABB)
		return;

	if (pTargetAABB->Get_Active()) {
		if (m_pAABBCom->Collsion_AABB(pTargetAABB)) {
			m_bDamaged = true;
			for (_uint i = 0; i <3; ++i) {
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_Spark"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
					return;
				}
			}
		}
	}

	list<CGameObject*>* pBulletList = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Bullet");

	if (pBulletList != nullptr) {
		for (auto iter : *pBulletList) {
			if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				m_bDamaged = true;
				break;
			}
				
		}

	} 

	//m_pOBBCom->Collsion_OBB(pTargetOBB);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBolterfly::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	if (!m_bDead) {
		for (_uint i = 0; i < iNumMeshContainers; ++i)
		{
			if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
				return E_FAIL;
		}
	}
	else {
		for (_uint i = 0; i < iNumMeshContainers; ++i)
		{
			if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 2)))
				return E_FAIL;
		}
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

HRESULT CBolterfly::Render_LightDepth()
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

HRESULT CBolterfly::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bolterfly"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vSize = _float3(0.7f, 0.7f, 0.7f);

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

HRESULT CBolterfly::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Set_RawValue("g_Count", &m_iDeadcount, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_DissolveTexture", 0)))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CBolterfly::HitSound()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->StopSound(CSound_Manager::PLAYER_WEAPON_HIT);
	switch (rand() % 4 + 1) {
	case 1: {
		pGameInstance->SoundPlay(L"Vorpal_Stab01.ogg", CSound_Manager::PLAYER_WEAPON_HIT, SOUND_DEFAULT);
	}
	case 2: {
		pGameInstance->SoundPlay(L"Vorpal_Stab02.ogg", CSound_Manager::PLAYER_WEAPON_HIT, SOUND_DEFAULT);
	}
	case 3: {
		pGameInstance->SoundPlay(L"Vorpal_Stab03.ogg", CSound_Manager::PLAYER_WEAPON_HIT, SOUND_DEFAULT);
	}
	case 4: {
		pGameInstance->SoundPlay(L"Vorpal_Stab04.ogg", CSound_Manager::PLAYER_WEAPON_HIT, SOUND_DEFAULT);
	}
	}
	RELEASE_INSTANCE(CGameInstance);
}

CBolterfly * CBolterfly::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CBolterfly*	pInstance = new CBolterfly(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBolterfly"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBolterfly::Clone(void * pArg)
{
	CBolterfly*	pInstance = new CBolterfly(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBolterfly"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBolterfly::Free()
{
	__super::Free();	
	//Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pAttAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
