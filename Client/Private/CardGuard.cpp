#include "stdafx.h"
#include "..\Public\CardGuard.h"
#include "GameInstance.h"

CCardGuard::CCardGuard(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CCardGuard::CCardGuard(const CCardGuard & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCardGuard::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCardGuard::NativeConstruct(void * pArg)
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
	

	m_pModelCom->Set_PreAnimIndex(21);
	m_pModelCom->Set_AnimationIndex(21);
	m_iAnimationIndex =21;
	m_pModelCom->Set_Cut(false);
	m_pModelCom->Set_Speed(1.f);
	m_pTransformCom->Rotation(XMVectorSet(0.f,1.f,0.f,0.f),XMConvertToRadians(180.f));

	//m_iCurrentCellIndex = 0;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -27.893f, 0.f, 1.f));
	//m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
	m_iCurrentCellIndex = 648;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-145.935f, -27.893f, 176.283f, 1.f));
	m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
	/*_float3 PlayerPos;
	XMStoreFloat3(&PlayerPos, m_pPlayerTrans->Get_State(CTransform::STATE_POSITION));
	_float3 MyPos;
	XMStoreFloat3(&MyPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	_vector Go = XMVectorSet(PlayerPos.x, PlayerPos.y, PlayerPos.z, 0.f) - XMVectorSet(MyPos.x, MyPos.y, MyPos.z, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(Go));
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));
	_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
*/
	m_pAABBCom->Set_Active(true);
	m_pAttAABBCom->Set_Active(false);

	CBlade::SOCKETDESC			SocketDesc;
	ZeroMemory(&SocketDesc, sizeof(CBlade::SOCKETDESC));

	return S_OK;
}

void CCardGuard::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	_float3 PlayerPos;
	XMStoreFloat3(&PlayerPos, m_pPlayerTrans->Get_State(CTransform::STATE_POSITION));
	_float3 MyPos;
	XMStoreFloat3(&MyPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	_float Dist = sqrtf((PlayerPos.x - MyPos.x)*(PlayerPos.x - MyPos.x) + (PlayerPos.y - MyPos.y)*(PlayerPos.y - MyPos.y) + (PlayerPos.z - MyPos.z)*(PlayerPos.z - MyPos.z));

	if (m_bSpawned == false && m_pPlayer->Get_CurrentIndex() == 641) {

		if (m_bSpawned == false) {
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			pGameInstance->StopSound(CSound_Manager::SCENE_SOUND1);
			pGameInstance->SoundPlay(L"CardGuard_Spawn.ogg", CSound_Manager::SCENE_SOUND1, SOUND_DEFAULT);
			RELEASE_INSTANCE(CGameInstance);
		}
		m_bSpawned = true;
		m_iAnimationIndex = 4;
		m_pModelCom->Set_Cut(true);
		m_pModelCom->Set_Speed(0.75f);
		m_pAABBCom->Set_Active(false);
		//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta*40.0);
		m_pPlayer->Set_PlayerMode(4);
		m_pPlayer->Set_Scene(2);
	}

	if (m_bSpawned == true && m_pModelCom->Get_ChangeEnd() && m_pModelCom->Get_Animation(4)->Get_KeyFrameENd() && m_pPlayer->Get_PlayerMode() != 4) {

		m_iAnimationIndex = 33;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
		m_bCutSceneEnd = true;
	}
	//}
	//if (m_bCutSceneEnd == true && m_pModelCom->Get_ChangeEnd() && m_pModelCom->Get_Animation(7)->Get_KeyFrameENd()) {
	//	m_iAnimationIndex =33;
	//	m_pModelCom->Set_Cut(true);
	//	m_pModelCom->Set_Speed(1.f);
	//	m_pAABBCom->Set_Active(false);
	//	m_bCutSceneEnd = true;


	if (m_bCutSceneEnd) {
		m_bDamaged = false;
		if (m_iHP == 0) {
			
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			if (m_iDeadcount == 0) {
				pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
				pGameInstance->SoundPlay(L"CardGuard_Death.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
			}
			RELEASE_INSTANCE(CGameInstance);
			++m_iDeadcount;
			m_bDead = true;
			m_iAnimationIndex = 18;
			m_pModelCom->Set_Cut(true);
			m_pModelCom->Set_Speed(1.f);
			m_pAABBCom->Set_Active(false);
			if (m_pModelCom->Get_Animation(18)->Get_KeyFrameENd()) {
				m_eState = STATE_DEAD;
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_CarriageCar", TEXT("Prototype_GameObject_CarriageCar")))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, Cloud");
					return;
				}
				RELEASE_INSTANCE(CGameInstance);
				m_pPlayer->Set_PlayerMode(4);
				m_pPlayer->Set_Scene(3);
			}
			
		}
		else {

			if (!m_iAttacking && Dist > 15.f)
			{
				m_iAttacking = 1;
			}
			else if (!m_iAttacking && Dist >= 3.f) {
				m_iAnimationIndex = 33;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta / 2, &m_iCurrentCellIndex, false);
			}
			else if (!m_iAttacking && Dist < 3.f) {
				m_iAttacking = 5;
			}

			//else if (!m_iAttacking && Dist < 1.f)
			//{
			//	if (rand() % 1000 == 0)
			//		m_iAttacking = 4;

			//	m_iAnimationIndex = 33;
			//	m_pModelCom->Set_Cut(false);
			//	m_pModelCom->Set_Speed(1.f);
			//	m_pTransformCom->Go_BackWardOnNavigation(TimeDelta / 4, &m_iCurrentCellIndex, false);
			//}

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
					m_iAnimationIndex = 7;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);

					if (m_pModelCom->Get_Animation(7)->Get_KeyFrameENd()) {
						m_iAttacking = 2;
						CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
						pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
						pGameInstance->SoundPlay(L"CardGuard_Swing1.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_WEIGHT);
						RELEASE_INSTANCE(CGameInstance);
					}
				}
				else if (m_iAttacking == 2) {
					m_pAABBCom->Set_Active(false);
					m_iAnimationIndex = 4;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					if (m_pModelCom->Get_Animation(4)->Get_KeyFrameENd()) {
						m_iAttacking = 3;
						CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

						if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_WeaponLamp", TEXT("Prototype_GameObject_WeaponLamp"), &m_pPlayerTrans->Get_State(CTransform::STATE_POSITION)))) {
							MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
							return;
						}

						RELEASE_INSTANCE(CGameInstance);
					}
				}
				else if (m_iAttacking == 3) {
					m_pAABBCom->Set_Active(true);
					m_iAnimationIndex = 5;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					if (m_pModelCom->Get_Animation(5)->Get_KeyFrameENd()) {
						m_iAttacking = 4;
					}
				}
				else if (m_iAttacking == 4) {
					m_pAABBCom->Set_Active(true);
					m_iAnimationIndex = 6;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					if (m_pModelCom->Get_Animation(6)->Get_KeyFrameENd()) {
						m_iAttacking = 0;
						_vector Go = XMVectorSet(PlayerPos.x, 0.f, PlayerPos.z, 0.f) - XMVectorSet(MyPos.x, 0.f, MyPos.z, 0.f);
						m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(Go));

						_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
						m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

						_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
						m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

					}
				}
				else if (m_iAttacking == 5) {
					m_pAABBCom->Set_Active(false);
					m_pAttAABBCom->Set_Active(false);
					m_iAnimationIndex = 3;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					if (m_pModelCom->Get_Animation(3)->Get_KeyFrameENd()) {
						m_iAttacking = 6;
						CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
						pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
						pGameInstance->SoundPlay(L"CardGuard_Swing2.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_WEIGHT);
						RELEASE_INSTANCE(CGameInstance);

					}
				}
				else if (m_iAttacking == 6) {
					m_pAABBCom->Set_Active(false);
					m_pAttAABBCom->Set_Active(true);
					m_iAnimationIndex = 0;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					if (m_pModelCom->Get_Animation(0)->Get_KeyFrameENd()) {
						m_iAttacking = 7;
						
					}
				}
				else if (m_iAttacking == 7) {
					m_pAABBCom->Set_Active(true);
					m_pAttAABBCom->Set_Active(false);
					m_iAnimationIndex = 1;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					if (m_pModelCom->Get_Animation(1)->Get_KeyFrameENd()) {
						m_iAttacking = 8;
					}
				}
				else if (m_iAttacking == 8) {
					m_pAABBCom->Set_Active(true);
					m_pAttAABBCom->Set_Active(false);
					m_iAnimationIndex = 2;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					if (m_pModelCom->Get_Animation(2)->Get_KeyFrameENd()) {
						m_iAttacking = 0;
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
	}
		_matrix AABBMat = m_pTransformCom->Get_WorldMatrix();
		AABBMat.r[3] += 0.3* m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		m_pAttAABBCom->Update(AABBMat);

		m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
		m_pModelCom->Set_AnimationIndex(m_iAnimationIndex);
		m_pModelCom->Update(TimeDelta);
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
		//m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CCardGuard::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*		pTargetAABB = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Weapon"), TEXT("Com_AABB"));
	if (nullptr == pTargetAABB)
		return;

	if (pTargetAABB->Get_Active() && m_pAABBCom->Get_Active()) {
		if (m_pAABBCom->Collsion_AABB(pTargetAABB)) {
			if (m_bDamaged == false && m_iHP >= 1) {
				if (m_pPlayer->Get_Super() && m_pPlayer->Get_Hp() < 79)
					m_pPlayer->Set_Damaged(-1);
				--m_iHP;
				for (_uint i = 0; i <3; ++i) {
					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_Spark"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
						return;
					}
				}
			}
			m_bDamaged = true;

		}
			
	}

	list<CGameObject*>* pBulletList = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Bullet");

	if (pBulletList != nullptr && m_pAABBCom->Get_Active()) {
		for (auto iter : *pBulletList) {
			if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				if (m_bDamaged == false && m_iHP >= 1)
					--m_iHP;
				iter->Set_Dead();
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

HRESULT CCardGuard::Render()
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

	return S_OK;

}

HRESULT CCardGuard::Render_LightDepth()
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

HRESULT CCardGuard::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CardGuard"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	ColliderDesc.vSize = _float3(1.4f, 4.f, 1.4f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vPosition = _float3(0.f, 3.5f,0.f);
	ColliderDesc.vSize = _float3(7.f, 7.f, 7.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AttAABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAttAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	//ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	//ColliderDesc.vSize = _float3(0.9f, 1.0f, 0.9f);

	//if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CCardGuard::SetUp_ConstantTable()
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

CCardGuard * CCardGuard::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CCardGuard*	pInstance = new CCardGuard(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCardGuard"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCardGuard::Clone(void * pArg)
{
	CCardGuard*	pInstance = new CCardGuard(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCardGuard"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCardGuard::Free()
{
	__super::Free();	
	//Safe_Release(m_pOBBCom);
	Safe_Release(m_pAttAABBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
