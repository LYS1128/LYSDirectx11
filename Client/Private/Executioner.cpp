#include "stdafx.h"
#include "..\Public\Executioner.h"
#include "GameInstance.h"

CExecutioner::CExecutioner(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CExecutioner::CExecutioner(const CExecutioner & rhs)
	: CGameObject(rhs)
{
}

HRESULT CExecutioner::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CExecutioner::NativeConstruct(void * pArg)
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
	pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
	m_pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
	m_pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));

	RELEASE_INSTANCE(CGameInstance);
	

	m_pModelCom->Set_PreAnimIndex(13);
	m_pModelCom->Set_AnimationIndex(13);
	m_iAnimationIndex =13;
	m_pModelCom->Set_Cut(true);
	m_pModelCom->Set_Speed(1.f);
	m_pTransformCom->Rotation(XMVectorSet(0.f,1.f,0.f,0.f),XMConvertToRadians(180.f));

	m_iCurrentCellIndex = 48;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.239f, 0.f, 86.421f, 1.f));
	m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);

	m_pAABBCom->Set_Active(true);
	m_pAttAABBCom->Set_Active(false);

	return S_OK;
}

void CExecutioner::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	_float3 PlayerPos;
	XMStoreFloat3(&PlayerPos, m_pPlayerTrans->Get_State(CTransform::STATE_POSITION));
	_float3 MyPos;
	XMStoreFloat3(&MyPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	_float Dist = sqrtf((PlayerPos.x - MyPos.x)*(PlayerPos.x - MyPos.x) + (PlayerPos.y - MyPos.y)*(PlayerPos.y - MyPos.y) + (PlayerPos.z - MyPos.z)*(PlayerPos.z - MyPos.z));

	if (m_bSpawned == false) {
		m_bSpawned = true;
		m_iAnimationIndex = 13;
		m_pModelCom->Set_Cut(true);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
	}

	if (m_bSpawned && m_pModelCom->Get_ChangeEnd() && m_pModelCom->Get_Animation(13)->Get_KeyFrameENd()) {
		m_bCutSceneEnd = true;
	}
	if (m_iHP <= 0) {
		m_eState = STATE_DEAD;
	}

	if(m_bCutSceneEnd){
		m_bDamaged = false;

		if (!m_iAttacking && Dist > 15.f)
		{
			m_iAttacking = 3;
		}
		else if (!m_iAttacking && Dist >= 3.f) {
			pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
			m_iAnimationIndex = 32;
			m_pModelCom->Set_Cut(false);
			m_pModelCom->Set_Speed(1.f);
			m_pTransformCom->Go_StraightOnNavigation(TimeDelta / 2, &m_iCurrentCellIndex, false);
		}
		else if (!m_iAttacking && Dist < 3.f) {
			m_iAttacking = 1;
		}

		if (!m_iAttacking) {
			_vector Go = XMVectorSet(PlayerPos.x, 0.f, PlayerPos.z, 0.f) - XMVectorSet(MyPos.x, 0.f, MyPos.z, 0.f);
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(Go));

			_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

			_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
			m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
		}

		else {
			if (m_iAttacking == 1) {
				m_pAABBCom->Set_Active(false);	
				m_iAnimationIndex = 3;
				m_pModelCom->Set_Cut(true);
				m_pModelCom->Set_Speed(1.f);

				if (m_pModelCom->Get_Animation(3)->Get_CurrnetKeyFrame() == 43) {
					pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
					pGameInstance->SoundPlay(L"Exe_Swing.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
				}


				if(m_pModelCom->Get_Animation(3)->Get_CurrnetKeyFrame() >= 44 && m_pModelCom->Get_Animation(3)->Get_CurrnetKeyFrame() <= 55)
					m_pAttAABBCom->Set_Active(true);
				else
					m_pAttAABBCom->Set_Active(false);
				if (m_pModelCom->Get_Animation(3)->Get_KeyFrameENd())
					m_iAttacking = 2;
			}
			else if (m_iAttacking == 2) {
				m_pAABBCom->Set_Active(true);
				m_pAttAABBCom->Set_Active(false);
				m_iAnimationIndex = 5;
				m_pModelCom->Set_Cut(true);
				m_pModelCom->Set_Speed(1.f);

				if (m_pModelCom->Get_Animation(5)->Get_CurrnetKeyFrame() == 22) {
					pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
					pGameInstance->SoundPlay(L"Exe_Grr.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
				}

				if (m_pModelCom->Get_Animation(5)->Get_KeyFrameENd()) {
					m_iAttacking = 0;
					_vector Go = XMVectorSet(PlayerPos.x, 0.f, PlayerPos.z, 0.f) - XMVectorSet(MyPos.x, 0.f, MyPos.z, 0.f);
					m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(Go));

					_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
					m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

					_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
					m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
				}
			}
		
			else if (m_iAttacking == 3) {
				
				pGameInstance->SoundPlay(L"Exe_Spin.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
				m_pAABBCom->Set_Active(false);
				m_pAttAABBCom->Set_Active(true);
				m_iAnimationIndex = 12;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta / 2, &m_iCurrentCellIndex, false);
				if (m_pModelCom->Get_Animation(12)->Get_KeyFrameENd()) {
					++m_iTime;
				}
				if (m_pModelCom->Get_ChangeEnd() && m_iTime >= 5) {
					m_iAttacking = 0;
					m_iTime = 0;
					m_pAABBCom->Set_Active(true);
					m_pAttAABBCom->Set_Active(false);
					_vector Go = XMVectorSet(PlayerPos.x, 0.f, PlayerPos.z, 0.f) - XMVectorSet(MyPos.x, 0.f, MyPos.z, 0.f);
					m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(Go));

					_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
					m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

					_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
					m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
				}
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);
		_matrix AABBMat = m_pTransformCom->Get_WorldMatrix();
		AABBMat.r[3] += 0.3* m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		m_pAttAABBCom->Update(AABBMat);

		m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
		m_pModelCom->Set_AnimationIndex(m_iAnimationIndex);
		m_pModelCom->Update(TimeDelta);
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
		//m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CExecutioner::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*		pTargetAABB = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Weapon"), TEXT("Com_AABB"));
	if (nullptr == pTargetAABB)
		return;

	if (pTargetAABB->Get_Active() && m_pAABBCom->Get_Active()) {
		if (m_pAABBCom->Collsion_AABB(pTargetAABB)) {
			if (m_bDamaged == false && m_iHP >= 1) {
				--m_iHP;
				HitSound();
				for (_uint i = 0; i <3; ++i) {
					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_Spark"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
						return;
					}
				}
				if (m_pPlayer->Get_Super() && m_pPlayer->Get_Hp() < 79)
					m_pPlayer->Set_Damaged(-1);
			}
			m_bDamaged = true;

		}
			
	}

	list<CGameObject*>* pTowerList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenTower");

	if (pTowerList != nullptr) {
		for (auto iter : *pTowerList) {
			if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				_float3 MySize = _float3(3.f, 6.f, 3.f);
				_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
				_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
				m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
			}
		}
	}

	list<CGameObject*>* pQueenList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenBase");

	if (pQueenList != nullptr) {
		for (auto iter : *pQueenList) {
			if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				_float3 MySize = _float3(3.f, 6.f, 3.f);
				_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
				_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
				m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
			}
		}
	}
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CExecutioner::Render()
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

HRESULT CExecutioner::Render_LightDepth()
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

HRESULT CExecutioner::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_Executioner"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	ColliderDesc.vSize = _float3(3.f, 6.f, 3.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vPosition = _float3(0.f, 4.f,0.f);
	ColliderDesc.vSize = _float3(8.f, 8.f, 8.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AttAABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAttAABBCom, &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CExecutioner::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CExecutioner::HitSound()
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


CExecutioner * CExecutioner::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CExecutioner*	pInstance = new CExecutioner(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CExecutioner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CExecutioner::Clone(void * pArg)
{
	CExecutioner*	pInstance = new CExecutioner(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CExecutioner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExecutioner::Free()
{
	__super::Free();	
	Safe_Release(m_pAttAABBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
