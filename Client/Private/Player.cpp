#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include "CardGuard.h"

CPlayer::CPlayer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{


	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	



	m_pModelCom->Set_PreAnimIndex(17);
	m_pModelCom->Set_AnimationIndex(17);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
	m_iCurrentCellIndex = 603;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-117.774f, -15.553f, 148.208f, 1.f));
	m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);


	return S_OK;
}

void CPlayer::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	if(m_iPlayerMode != 4)
		Key_Input(TimeDelta);

	if (m_iPlayerMode == 2)
		g_iMode = PMODE_SMALL;
	else if (m_bSuper)
		g_iMode = PMODE_SUPER;
	else
		g_iMode = PMODE_DEFAULT;


	m_pModelCom->Set_AnimationIndex(m_iAnimationIndex);
	m_pModelCom->Update(TimeDelta);

	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	//m_pOBBCom->Update(m_pTransformCom->Get_WorldM4trix());
	//m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CPlayer::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (g_iLevel == LEVEL_GAMEPLAY) {
		list<CGameObject*>* pMonAttList = pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_Monster");

		if (pMonAttList != nullptr && m_bRush == false) {
			for (auto iter : *pMonAttList) {
				if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
					_float3 MySize = _float3(0.5f, 1.65f, 0.5f);
					_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
					_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
					m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
				}
				if (dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AttAABB"))->Get_Active() && m_pAABBCom->Get_Active()) {
					if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AttAABB"))) && m_iHp >= 1 && m_bSuper == false) {
						--m_iHp;
						m_bDamaged = true;
					}
				}
			}
		}

		list<CGameObject*>* pCardGList = pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_CardGuard");

		if (pCardGList != nullptr && m_bRush == false) {
			for (auto iter : *pCardGList) {
				if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
					_float3 MySize = _float3(0.5f, 1.65f, 0.5f);
					_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
					_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
					m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
				}
				if (dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AttAABB"))->Get_Active() && m_pAABBCom->Get_Active() && m_bSuper == false) {
					if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AttAABB")))) {
						if (m_iHp >= 5)
							m_iHp -= 5;
						else
							m_iHp = 0;
						m_bDamaged = true;
					}
				}
			}
		}

		list<CGameObject*>* pLampGList = pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_WeaponLamp");

		if (pLampGList != nullptr && m_bRush == false) {
			for (auto iter : *pLampGList) {
				if (dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AttAABB"))->Get_Active() && m_pAABBCom->Get_Active() && m_bSuper ==false) {
					if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AttAABB")))) {
						if (m_iHp >= 10)
							m_iHp -= 10;
						else
							m_iHp = 0;
						m_bDamaged = true;
					}
				}
			}
		}

		list<CGameObject*>* pObjectList = pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_Object");

		if (pObjectList != nullptr) {
			for (auto iter : *pObjectList) {
				if (m_iPlayerMode != 3 && m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
					_float3 MySize = _float3(0.5f, 1.65f, 0.5f);
					_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
					_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
					m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
				}
			}
		}
	}
	else if (g_iLevel == LEVEL_BOSS) {
		list<CGameObject*>* pExeGList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Executioner");

		if (pExeGList != nullptr && m_bRush == false) {
			for (auto iter : *pExeGList) {
				if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
					_float3 MySize = _float3(0.5f, 1.65f, 0.5f);
					_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
					_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
					m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
				}
				if (dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AttAABB"))->Get_Active() && m_pAABBCom->Get_Active() && m_bSuper == false) {
					if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AttAABB")))) {
						if (m_iHp >= 5)
							m_iHp -= 5;
						else
							m_iHp = 0;
						m_bDamaged = true;
					}
				}
			}
		}

		list<CGameObject*>* pMonAttList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Knight");

		if (pMonAttList != nullptr && m_bRush == false) {
			for (auto iter : *pMonAttList) {
				if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
					_float3 MySize = _float3(0.5f, 1.65f, 0.5f);
					_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
					_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
					m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
				}
			}
		}

		list<CGameObject*>* pTowerList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenTower");

		if (pTowerList != nullptr && m_bRush == false) {
			for (auto iter : *pTowerList) {
				if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
					_float3 MySize = _float3(0.5f, 1.65f, 0.5f);
					_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
					_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
					m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
				}
			}
		}

		list<CGameObject*>* pQueenList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenBase");

		if (pQueenList != nullptr && m_bRush == false) {
			for (auto iter : *pQueenList) {
				if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
					_float3 MySize = _float3(0.5f, 1.65f, 0.5f);
					_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
					_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
					m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
				}
			}
		}

	}

	RELEASE_INSTANCE(CGameInstance);
	if (!m_bRush && !m_bDropDead && (m_iPlayerMode != 4 || (m_iPlayerMode == 4 && m_iScene == 4))) {
		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	}
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (g_iLevel == LEVEL_BOSS) {
		if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado") != nullptr && pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->size() != 0) {
			if (dynamic_cast<CTornado*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->front())->Get_Red())
				m_iShaderPath = 3;
			else
				m_iShaderPath = 4;
		}
		else
			m_iShaderPath = 0;
	}
	else
		m_iShaderPath = 0;
	RELEASE_INSTANCE(CGameInstance);

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, m_iShaderPath)))
			return E_FAIL;
	}	
//#ifdef _DEBUG
//	m_pAABBCom->Render();
//	//m_pOBBCom->Render();
//	//m_pSphereCom->Render();
//#endif // _DEBUG

	return S_OK;
}

HRESULT CPlayer::Render_LightDepth()
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
	_vector vLightPos = -2.0f*180.0f*vLightDir + XMVectorSet(0.f, 0.f, 0.f, 1.f); // 맵전체감싸는원반지름 180
	_vector vTargetPos = XMVectorSet(-20.f, 26.f, 146.f, 1.f); //맵 중심좌표
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



HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SK_Alice"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.825f, 0.f);
	ColliderDesc.vSize = _float3(0.5f, 1.65f, 0.5f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	m_pAABBCom->Set_Active(true);

	return S_OK;
}

HRESULT CPlayer::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	_vector CamPos = pGameInstance->Get_CamPosition();
	if (FAILED(m_pShaderCom->Set_RawValue("g_CamDirection", &CamPos, sizeof(_vector))))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



void CPlayer::Key_Input(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region 슬라이드...

	if (m_pTransformCom->NaviOption(&m_iCurrentCellIndex) == 4) {

		if(m_fSlideTime < 3.f)
			m_fSlideTime += TimeDelta*0.1;

		if (m_SlideSound == false) {
			m_SlideSound = true;
			pGameInstance->StopSound(CSound_Manager::PLAYER_FOOTSTEPR);
		}

		pGameInstance->SoundPlay(L"Alice_Slide.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_WEIGHT);

		m_iPlayerMode = 3;
		m_pTransformCom->Go_StraightOnNavigation(m_fSlideTime * TimeDelta, &m_iCurrentCellIndex, m_bJumping, true, m_bSmall);
		m_iAnimationIndex = 2;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);

		if (pGameInstance->Key_Pressing('A')) {
			if (m_fSlideTime - TimeDelta * 0.3> 1.f)
				m_fSlideTime -= TimeDelta* 0.3;

			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -TimeDelta);
		}
		else if (pGameInstance->Key_Pressing('D')) {

			if (m_fSlideTime - TimeDelta* 0.1 > 1.f)
				m_fSlideTime -= TimeDelta * 0.1;
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);
		}

		RELEASE_INSTANCE(CGameInstance);
		return;
	}
	else if (m_iPlayerMode == 3 && m_pTransformCom->NaviOption(&m_iCurrentCellIndex) != 4) {
		m_SlideSound = false;
		m_iPlayerMode = 0;
		m_fSlideTime = 1.f;
	}
		

#pragma endregion

#pragma region 가만히 있는 상태 
	if (!m_iAttacking && !m_bJumping && m_pModelCom->Get_ChangeEnd()) {

		if (m_iPlayerMode == 0) //칼모드
			m_iAnimationIndex = 37;
		else if (m_iPlayerMode == 1) //총모드
			m_iAnimationIndex = 51;
		else //소인화모드
			m_iAnimationIndex = 17;

		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_bMoving = false;
	}

	if (m_iAttacking == 0 || m_iPlayerMode != 1) { //총 식히는거
		if (m_fGunfireTime > 0.f)
			m_fGunfireTime -= TimeDelta / 2.f;
		else
			m_fGunfireTime = 0.f;
	}

#pragma endregion

#pragma region 낙사

	if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < m_pTransformCom->Compute_Height(&m_iCurrentCellIndex) - 20.f || m_iHp <= 0)
		m_bDropDead = true;

	if (m_bDropDead) {
		
		if (m_fDropDeadTime == 0.0f) {
			pGameInstance->StopSound(CSound_Manager::PLAYER_BREATH);
		}

		m_fDropDeadTime += TimeDelta;

		if (m_fDropDeadTime < 0.8f) {
			pGameInstance->SoundPlay(L"Alice_Death.ogg", CSound_Manager::PLAYER_BREATH, SOUND_DEFAULT);
			for (_uint i = 0; i < 10; ++i) {
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_StartButterfly")))) {
					MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,RushButterfly"));
					return;
				}
			}
		}
		else if (m_fDropDeadTime >= 1.5f && m_bRespawn == false) {
			pGameInstance->StopSound(CSound_Manager::PLAYER_BREATH);
			m_bRespawn = true;

			if (g_iLevel == LEVEL_BOSS) {
				if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado") != nullptr && pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->size() != 0) {
					
					for(auto& iter: *pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado"))
						iter->Set_Dead();
					m_iShaderPath = 0;
				}
			}

			if (m_iCurrentCellIndex < 238) {
				m_iCurrentCellIndex = 0;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.5f, 0.f, 1.f));
				m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
			}
			else if (m_iCurrentCellIndex >= 238 && m_iCurrentCellIndex < 512) {
				m_iCurrentCellIndex = 238;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(52.392f,41.82f,143.67f, 1.f));
				m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
			}
			else if (m_iCurrentCellIndex >=  512) {
				m_iCurrentCellIndex = 512;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-81.375f,-31.035f, 174.3f, 1.f));
				m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
			}

			if (pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_CardGuard") != nullptr && pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_CardGuard")->size() != 0) {
				if (dynamic_cast<CCardGuard*>(pGameInstance->Find_Layer_List(LEVEL_GAMEPLAY, L"Layer_CardGuard")->front())->Get_CutSceneEnd()) {
					m_iCurrentCellIndex = 644;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-138.658f, -27.893f, 169.899f, 1.f));
					m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
				}
			}
		}
		else if (m_fDropDeadTime > 1.5f && m_fDropDeadTime < 1.8f) {
			pGameInstance->SoundPlay(L"Alice_Respawn.ogg", CSound_Manager::PLAYER_BREATH, SOUND_DEFAULT);
			for (_uint i = 0; i < 20; ++i) {
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_RushButterfly")))) {
					MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,RushButterfly"));
					return;
				}
			}

		}
		else if(m_fDropDeadTime >= 1.8f){
			if (g_iLevel == LEVEL_BOSS) {
				if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado") != nullptr && pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->size() != 0) {
					pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->front()->Set_Dead();
					m_iShaderPath = 0;
				}
			}

			m_iHp = 79;
			m_iShaderPath = 0;
			m_bDamaged = false;
			m_bRespawn = false;
			m_fDropDeadTime = 0.f;
			m_bDropDead = false;
			m_bMosAttach = false;
			m_fAttachTime = 0.f;
		}
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

#pragma endregion

#pragma region 무적모드
	if (m_iHp < 40 && m_iAttacking == 0 && m_iPlayerMode != 1 && m_iPlayerMode != 2 && pGameInstance->Key_Down(VK_RETURN)) {
		m_bSuper = true;

		pGameInstance->StopSound( CSound_Manager::PLAYER_BREATH);
		pGameInstance->SoundPlay(L"Hysteria.ogg", CSound_Manager::PLAYER_BREATH, SOUND_DEFAULT);
		
	}

	if (m_bSuper) {
		if (!m_bSuperOn) {
			m_bSuperStart = true;
			m_iAnimationIndex = 16;
			m_pModelCom->Set_Cut(true);
			m_pModelCom->Set_Speed(1.f);
			m_pAABBCom->Set_Active(false);
			if (m_pModelCom->Get_Animation(16)->Get_KeyFrameENd()) {
				m_bSuperOn = true;
				m_bSuperStart = false;
			}
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		m_bMosAttach = false;
		m_bDamaged = false;
		m_pAABBCom->Set_Active(false);
		m_iPlayerMode = 0;
	}
	else {
		m_pAABBCom->Set_Active(true);
		m_bSuperOn = false;
	}
#pragma endregion

#pragma region 공격받음
	if (m_bMosAttach == true) {
		m_iPlayerMode = 0;
		m_iAnimationIndex = 9;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
		m_fAttachTime += TimeDelta;
		if (m_fAttachTime > 1.f && m_iHp >= 1) {
			m_fAttachTime = 0.f;
			--m_iHp;
		}
	}
	else
		m_fAttachTime = 0.f;

	if (m_bDamaged == true) {
		m_iAnimationIndex = 13;
		m_pModelCom->Set_Cut(true);
		m_pModelCom->Set_Speed(1.f);
		m_pAABBCom->Set_Active(false);
		m_RushTime = 0;
		m_bRush = false;
		m_pTransformCom->Go_BackWardOnNavigation(TimeDelta / 4, &m_iCurrentCellIndex, m_bJumping, true, m_bSmall);
		if (m_pModelCom->Get_Animation(13)->Get_CurrnetKeyFrame() == 1)
			DamageSound(TimeDelta);
		else if (m_pModelCom->Get_Animation(13)->Get_KeyFrameENd()) {
			m_bDamaged = false;
			m_pAABBCom->Set_Active(true);
		}
		RELEASE_INSTANCE(CGameInstance);
		return;

	}
#pragma endregion

#pragma region 소인화모드(총모드이거나 칼모드로 공격중일때는 불가능)
	if (m_bMosAttach == false && m_iAttacking == 0 && m_iPlayerMode != 1 && pGameInstance->Key_Down(VK_LCONTROL)) {
		if (m_iPlayerMode == 0) {
			pGameInstance->StopSound(CSound_Manager::PLAYER_BREATH);
			pGameInstance->SoundPlay(L"Alice_Drink.ogg", CSound_Manager::PLAYER_BREATH, SOUND_HALF);
			m_bSmall = true;
			m_iPlayerMode = 2;
		}
		else {
			m_bFlower = false;
			m_bSmall = false;
			m_iPlayerMode = 0;
		}
	}

	if (m_iPlayerMode == 2) {
		HiccupSound(TimeDelta);
		m_pTransformCom->Scaled(_float3(0.5f, 0.5f, 0.5f));
	}
	else
		m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
#pragma endregion

#pragma region 점프

	if (m_iPlayerMode == 0) {

		if (m_bJumping == true && m_iPlayerMode != 4) {

 			if (m_pModelCom->Get_ChangeEnd() && ((m_iJumpNum == 1 && m_bDest3) || (m_bDest && m_iJumpNum == 2) || (m_bDest2 && m_iJumpNum == 3) || m_bSuperJump)) {

				m_iAnimationIndex = 19;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
			}
			if(m_iJumpNum == 1 && m_pModelCom->Get_Animation(19)->Get_KeyFrameENd())
				m_bDest3 = true;
			if (m_iJumpNum == 2 && m_bDest3 && m_pModelCom->Get_Animation(25)->Get_KeyFrameENd())
				m_bDest = true;
			if (m_iJumpNum == 3 && m_bDest && m_pModelCom->Get_Animation(29)->Get_KeyFrameENd())
				m_bDest2 = true;

			_vector  vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector	vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
			if(m_bSuperJump && m_fJumpTime <= 2.5f)
				vPosition += 6.f * XMVector3Normalize(vUp) * (2.5f - (m_fJumpTime)) * 0.1f;
			else
				vPosition += XMVector3Normalize(vUp) * (2.5f - (m_fJumpTime)) * 0.1f;
			if (XMVectorGetY(vPosition) > m_pTransformCom->Compute_Height(&m_iCurrentCellIndex) || m_pTransformCom->NaviOption(&m_iCurrentCellIndex) == 2) {
				if (!m_bDropDead) {
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
					if (pGameInstance->Key_Pressing(VK_SPACE) && m_fJumpTime >= 2.5f) {

						if (m_bFall == false) {
							pGameInstance->StopSound(CSound_Manager::PLAYER_JUMP);
							pGameInstance->SoundPlay(L"JumpGlide.ogg", CSound_Manager::PLAYER_JUMP, SOUND_WEIGHT);
							if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_FallStroke"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
								MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
								return;
							}
						}

							if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_FallFeather"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
								MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
								return;
						}
						m_bFall = true;
						m_fJumpTime += 0.01f;
					}					
					else {
						pGameInstance->StopSound(CSound_Manager::PLAYER_JUMP);
						m_bFall = false;
						m_fJumpTime += 0.1f;
					}
				}
			}
			else {
				pGameInstance->StopSound(CSound_Manager::PLAYER_JUMP);
				m_bFall = false;
				m_bDest = false;
				m_bDest2 = false;
				m_bDest3 = false;
				m_iJumpNum = 0;
				m_bSuperJump = false;
				m_bJumping = false;
				m_fJumpTime = 0.f;
			}
		}
		else {
			if (m_pTransformCom->NaviOption(&m_iCurrentCellIndex) != 2 && XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) > m_pTransformCom->Compute_Height(&m_iCurrentCellIndex)- 1.f) {
				m_bFall = false;
				m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
				m_fDropTime = 0.f;
			}
				
			else {
				if (!m_bDropDead) {
					_vector  vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
					_vector	vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
					vPosition += XMVector3Normalize(vUp) * -m_fDropTime * 0.1f;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
					m_fDropTime += 0.1f;
				}
			}
		}
		if (!m_iAttacking&&pGameInstance->Key_Down(VK_SPACE)) {
			switch (m_iJumpNum) {
			case 0: {
				if (m_pModelCom->Get_ChangeEnd()) {

					pGameInstance->StopSound(CSound_Manager::PLAYER_JUMP);
					pGameInstance->SoundPlay(L"Jump1.ogg", CSound_Manager::PLAYER_JUMP, SOUND_DEFAULT);
					pGameInstance->StopSound(CSound_Manager::PLAYER_BREATH);
					pGameInstance->SoundPlay(L"Alice_Jump1.ogg", CSound_Manager::PLAYER_BREATH, SOUND_WEIGHT);
					m_iAnimationIndex = 19;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					m_bDest = false;
					m_bDest2 = false;
					m_bDest3 = false;
					m_bSuperJump = false;
					m_bJumping = true;
					m_bFall = false;
					++m_iJumpNum;
					break;
				}

			}
			case 1: {
  				if (m_pModelCom->Get_ChangeEnd()) {
					pGameInstance->StopSound(CSound_Manager::PLAYER_JUMP);
					pGameInstance->SoundPlay(L"Jump2.ogg", CSound_Manager::PLAYER_JUMP, SOUND_DEFAULT);
					pGameInstance->StopSound(CSound_Manager::PLAYER_BREATH);
					pGameInstance->SoundPlay(L"Alice_Jump2.ogg", CSound_Manager::PLAYER_BREATH, SOUND_WEIGHT);
  					m_iAnimationIndex = 25;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					m_bDest = false;
					m_bDest2 = false;
					m_bDest3 = false;
					m_bSuperJump = false;
					m_fJumpTime = 0.f;
					m_bJumping = true;
					m_bFall = false;
					++m_iJumpNum;

					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_WindStroke"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
						return;
					}
					for (_uint i = 0; i < 10; ++i) {
						if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_JumpFeather"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
							MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
							return;
						}
					}
					break;
				}
	
			}
			case 2: {
				if (m_pModelCom->Get_ChangeEnd()) {
					pGameInstance->StopSound(CSound_Manager::PLAYER_JUMP);
					pGameInstance->SoundPlay(L"Jump3.ogg", CSound_Manager::PLAYER_JUMP, SOUND_DEFAULT);
					pGameInstance->StopSound(CSound_Manager::PLAYER_BREATH);
					pGameInstance->SoundPlay(L"Alice_Jump3.ogg", CSound_Manager::PLAYER_BREATH, SOUND_WEIGHT);
					m_iAnimationIndex = 29;
					m_pModelCom->Set_Cut(true);
					m_pModelCom->Set_Speed(1.f);
					m_bDest = false;
					m_bDest2 = false;
					m_bDest3 = false;
					m_bSuperJump = false;
					m_bJumping = true;
					m_fJumpTime = 0.f;
					++m_iJumpNum;

					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_WindStroke"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
						return;
					}

					for (_uint i = 0; i < 10; ++i) {
						if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_JumpFeather"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
							MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
							return;
						}
					}

					break;
				}

			}
			}
		}
	}
	else {
		if (m_pTransformCom->NaviOption(&m_iCurrentCellIndex) != 2) {
			m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);
			m_fDropTime = 0.f;
		}

		else {
			if (!m_bDropDead) {
				_vector  vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_vector	vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
				vPosition += XMVector3Normalize(vUp) * -m_fDropTime * 0.1f;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
				m_fDropTime += 0.1f;
			}
		}
	}
#pragma endregion

#pragma region 대쉬
	if (m_iPlayerMode == 0) {
		if (!m_iAttacking&&pGameInstance->Key_Down(VK_SHIFT)) {
			m_bRush = true;
			m_bMosAttach = false;
			pGameInstance->StopSound(CSound_Manager::PLAYER_FOOTSTEPL);
			pGameInstance->SoundPlay(L"Alice_Dash.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_HALF);
		}
	}
	if (m_iPlayerMode == 0) {
		if (m_bRush == true)
		{
			for (_uint i = 0; i < 2; ++i) {
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_RushButterfly")))) {
					MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,RushButterfly"));
					return;
				}
			}

			m_RushTime += TimeDelta;
			//m_pTransformCom->Go_Straight(TimeDelta*4.0);
			m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 4, &m_iCurrentCellIndex,m_bJumping, true,m_bSmall);
			if (m_RushTime <= 0.2) {
				for (_uint i = 0; i < 5; ++i) {
					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_StartButterfly")))) {
						MSG_BOX(TEXT("Failed to Created CLevel_Logo: Ready_Layer_SubUI,RushButterfly"));
						return;
					}
				}
			}
			if (m_RushTime >= 0.5)
			{
				m_RushTime = 0;
				m_pAABBCom->Set_Active(true);
				m_bRush = false;
			}
		}
	}
#pragma endregion

#pragma region 움직이기
	if (!m_bFlower && !m_bMosAttach && m_iPlayerMode != 1 && !m_bRush) { //총모드 아닐시
		if (m_iAttacking == false && m_bMoving == false && (pGameInstance->Key_Down('W') || pGameInstance->Key_Down('S') || pGameInstance->Key_Down('A') || pGameInstance->Key_Down('D'))) {
			CTransform* pTargetTransform = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Camera", L"Com_Transform"));
			Safe_AddRef(pTargetTransform);

			_float3 fCamLook;
			XMStoreFloat3(&fCamLook, pTargetTransform->Get_State(CTransform::STATE_LOOK));
			_vector vNewLook = XMVector3Normalize(XMVectorSet(fCamLook.x, 0.f, fCamLook.z, 0.f));
			if (XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMVector3Cross(XMVectorSet(0.f, 0.f, 1.f, 0.f), vNewLook))) < 0)
				m_fAngle = -acos(XMVectorGetX(XMVector3Dot(vNewLook, XMVectorSet(0.f, 0.f, 1.f, 0.f))));
			else
				m_fAngle = acos(XMVectorGetX(XMVector3Dot(vNewLook, XMVectorSet(0.f, 0.f, 1.f, 0.f))));

			Safe_Release(pTargetTransform);
		}
		if (!m_iAttacking&&pGameInstance->Key_Pressing('W')) {
			if (m_pModelCom->Get_ChangeEnd() && !m_bJumping) {
				if (m_iPlayerMode == 0)
					m_iAnimationIndex = 42;
				else
					m_iAnimationIndex = 34;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_bMoving = true;
			}

			if (pGameInstance->Key_Pressing('A')) {
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle + XMConvertToRadians(-45.0f));
				//m_pTransformCom->Go_Straight(2.f*TimeDelta);
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 2, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);
			}
			else if (pGameInstance->Key_Pressing('D')) {
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle + XMConvertToRadians(45.0f));
				//m_pTransformCom->Go_Straight(2.f*TimeDelta);
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 2, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);
			}
			else {
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle + XMConvertToRadians(0.0f));
				//m_pTransformCom->Go_Straight(2.f*TimeDelta);
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 2, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);
			}
			if(!m_bJumping)
				WalkSound(TimeDelta);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if (!m_iAttacking&&pGameInstance->Key_Pressing('S')) {
			if (m_pModelCom->Get_ChangeEnd() && !m_bJumping) {
				if (m_iPlayerMode == 0)
					m_iAnimationIndex = 42;
				else
					m_iAnimationIndex = 34;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_bMoving = true;
			}

			if (pGameInstance->Key_Pressing('A')) {
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle + XMConvertToRadians(-135.0f));
				//m_pTransformCom->Go_Straight(2.f*TimeDelta);
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 2, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);
			}
			else if (pGameInstance->Key_Pressing('D')) {
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle + XMConvertToRadians(135.0f));
				//m_pTransformCom->Go_Straight(2.f*TimeDelta);
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 2, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);
			}
			else {
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle + XMConvertToRadians(180.0f));
				//m_pTransformCom->Go_Straight(2.f*TimeDelta);
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 2, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);
			}
			if (!m_bJumping)
				WalkSound(TimeDelta);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if (!m_iAttacking&&pGameInstance->Key_Pressing('A')) {
			if (m_pModelCom->Get_ChangeEnd() && !m_bJumping) {
				if (m_iPlayerMode == 0)
					m_iAnimationIndex = 42;
				else
					m_iAnimationIndex = 34;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_bMoving = true;
			}

			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle + XMConvertToRadians(-90.0f));
			//m_pTransformCom->Go_Straight(2.f*TimeDelta);
			m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 2, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);
			if (!m_bJumping)
				WalkSound(TimeDelta);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if (!m_iAttacking&&pGameInstance->Key_Pressing('D')) {
			if (m_pModelCom->Get_ChangeEnd() && !m_bJumping) {
				if (m_iPlayerMode == 0)
					m_iAnimationIndex = 42;
				else
					m_iAnimationIndex = 34;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_bMoving = true;
			}

			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle + XMConvertToRadians(90.0f));
			//m_pTransformCom->Go_Straight(2.f*TimeDelta);
			m_pTransformCom->Go_StraightOnNavigation(TimeDelta * 2, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);

			if (!m_bJumping)
				WalkSound(TimeDelta);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	else if (!m_bMosAttach && m_iPlayerMode == 1 && !m_bRush) { //총모드일시

		_long		MouseMove;

		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);
		}

		if (!m_iAttacking&&pGameInstance->Key_Pressing('A')) {
			if (m_pModelCom->Get_ChangeEnd()) {
				m_iAnimationIndex = 56;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_bMoving = true;
			}
			m_pTransformCom->Go_LeftOnNavigation(TimeDelta, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);

			if (!m_bJumping)
				WalkSound(TimeDelta);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		else if (!m_iAttacking&&pGameInstance->Key_Pressing('D')) {
			if (m_pModelCom->Get_ChangeEnd()) {
				m_iAnimationIndex = 57;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_bMoving = true;
			}


			m_pTransformCom->Go_RightOnNavigation(TimeDelta, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);

			if (!m_bJumping)
				WalkSound(TimeDelta);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		else if (!m_iAttacking&&pGameInstance->Key_Pressing('W')) {
			if (m_pModelCom->Get_ChangeEnd()) {
				m_iAnimationIndex = 55;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_bMoving = true;
			}
			m_pTransformCom->Go_StraightOnNavigation(TimeDelta, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);

			if (!m_bJumping)
				WalkSound(TimeDelta);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		else if (!m_iAttacking&&pGameInstance->Key_Pressing('S')) {
			if (m_pModelCom->Get_ChangeEnd()) {
				m_iAnimationIndex = 57;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_bMoving = true;
			}


			m_pTransformCom->Go_BackWardOnNavigation(TimeDelta, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);
			if (!m_bJumping)
				WalkSound(TimeDelta);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
#pragma endregion

#pragma region 칼모드 공격
	m_bSwardAABB = false;

	if (m_iPlayerMode == 0) {
		if (m_iAttacking == 3) {
			if (m_pModelCom->Get_ChangeEnd() && !m_bJumping)
				m_iAnimationIndex = 41;

			if ((m_pModelCom->Get_Animation(41)->Get_CurrnetKeyFrame() > 6 && m_pModelCom->Get_Animation(41)->Get_CurrnetKeyFrame() < 18))
				m_pTransformCom->Go_StraightOnNavigation(2.f*TimeDelta, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);

			if (m_pModelCom->Get_Animation(41)->Get_CurrnetKeyFrame() >= 13 && m_pModelCom->Get_Animation(41)->Get_CurrnetKeyFrame() <= 17)
				m_bSwardAABB = true;

			if (m_pModelCom->Get_Animation(41)->Get_KeyFrameENd()) {
				m_bTriAttacking = false;
				m_iAttacking = 0;
			}
		}

		if (m_iAttacking == 2)
		{
			if (m_pModelCom->Get_ChangeEnd() && !m_bJumping)
				m_iAnimationIndex = 40;

			if (pGameInstance->Key_Down(VK_LBUTTON))
				m_bTriAttacking = true;

			if (m_pModelCom->Get_Animation(40)->Get_CurrnetKeyFrame() >= 13 && m_pModelCom->Get_Animation(40)->Get_CurrnetKeyFrame() <= 17)
				m_bSwardAABB = true;

			if (m_pModelCom->Get_Animation(40)->Get_KeyFrameENd()) {
				m_bComboAttacking = false;
				if (m_bTriAttacking == false)
					m_iAttacking = 0;
				else {
					_uint i = 3;
					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_StrikeLine"), &i))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, StrikeLine");
						return;
					}
					VopalSound(TimeDelta);
					m_iAttacking = 3;
				}
			}
		}

		if (m_iAttacking == 1)
		{
			if (pGameInstance->Key_Down(VK_LBUTTON) && !m_bJumping)
				m_bComboAttacking = true;

			if (m_bComboAttacking && pGameInstance->Key_Down(VK_LBUTTON))
				m_bTriAttacking = true;

			if (m_pModelCom->Get_Animation(39)->Get_CurrnetKeyFrame() < 5 || (m_pModelCom->Get_Animation(39)->Get_CurrnetKeyFrame() > 10 && m_pModelCom->Get_Animation(39)->Get_CurrnetKeyFrame() < 13))
				m_pTransformCom->Go_StraightOnNavigation(TimeDelta, &m_iCurrentCellIndex, m_bJumping,true, m_bSmall);

			if (m_pModelCom->Get_Animation(39)->Get_CurrnetKeyFrame() >= 12 && m_pModelCom->Get_Animation(39)->Get_CurrnetKeyFrame() <= 14)
				m_bSwardAABB = true;

			if (m_pModelCom->Get_Animation(39)->Get_KeyFrameENd()) {
				if (m_bComboAttacking == false)
					m_iAttacking = 0;
				else {
					_uint i = 2;
					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_StrikeLine"), &i))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, StrikeLine");
						return;
					}
					VopalSound(TimeDelta);
					m_iAttacking = 2;
				}
			}
		}
	}
		if (m_bMosAttach == false && m_iPlayerMode !=2 && m_pModelCom->Get_ChangeEnd() && m_iAttacking == 0 && pGameInstance->Key_Down(VK_LBUTTON) && !m_bJumping)
		{
			VopalSound(TimeDelta);
			_uint i = 1;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Effect", TEXT("Prototype_GameObject_StrikeLine"),&i))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, StrikeLine");
				return;
			}
			m_iPlayerMode = 0;
			m_iAnimationIndex = 39;
			m_pModelCom->Set_Cut(true);
			m_pModelCom->Set_Speed(4.f);
			m_iAttacking = 1;
		}

#pragma endregion

#pragma region 총모드 공격

		if (m_bMosAttach == false && m_iPlayerMode != 2 && m_pModelCom->Get_ChangeEnd() && pGameInstance->Key_Pressing(VK_RBUTTON))
		{
			
			m_fGunfireTime += TimeDelta;

			if (m_fGunfireTime >= 5.f) {
				m_fGunfireTime = 5.f;
				m_iPlayerMode = 1;
				m_iAnimationIndex = 52;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);

				if (m_pModelCom->Get_Animation(52)->Get_KeyFrameENd()) {
					pGameInstance->StopSound(CSound_Manager::PLAYER_WEAPON);
					pGameInstance->SoundPlay(L"Pepperg_Noammo.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_DEFAULT);
				}

			}
			else {
				++m_iGunTickCount;

				pGameInstance->SoundPlay(L"Pepperg_Grind.ogg", CSound_Manager::PLAYER_WEAPON_SUB, SOUND_WEIGHT);

				if (m_iGunTickCount % 10 == 0) {
					GunFireSouund(TimeDelta);
					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Bullet", TEXT("Prototype_GameObject_Bullet")))) {
						MSG_BOX(L"총알안나감");
						return;
					}
				}

				m_iPlayerMode = 1;
				m_iAnimationIndex = 50;
				m_pModelCom->Set_Cut(false);
				m_pModelCom->Set_Speed(1.f);
				m_iAttacking = 1;
			}
		}
		else if (pGameInstance->Key_Up(VK_RBUTTON)) {
			pGameInstance->StopSound(CSound_Manager::PLAYER_WEAPON_SUB);
			m_iGunTickCount = 0;
			m_iAttacking = 0;
		}


		
			
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::WalkSound(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_LFootStepTime += TimeDelta;
	m_RFootStepTime += TimeDelta;

	if (m_LFootStepTime > 0.5) {
		m_LFootStepTime = 0.0;
		pGameInstance->StopSound(CSound_Manager::PLAYER_FOOTSTEPL);
		switch (rand() % 9 + 1) {
		case 1: {
			pGameInstance->SoundPlay(L"Walk1.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		case 2: {
			pGameInstance->SoundPlay(L"Walk2.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		case 3: {
			pGameInstance->SoundPlay(L"Walk3.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		case 4: {
			pGameInstance->SoundPlay(L"Walk4.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		case 5: {
			pGameInstance->SoundPlay(L"Walk5.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		case 6: {
			pGameInstance->SoundPlay(L"Walk6.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		case 7: {
			pGameInstance->SoundPlay(L"Walk7.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		case 8: {
			pGameInstance->SoundPlay(L"Walk8.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		case 9: {
			pGameInstance->SoundPlay(L"Walk9.ogg", CSound_Manager::PLAYER_FOOTSTEPL, SOUND_THREE);
		}
		}
	}
	if (m_RFootStepTime > 0.5) {
		m_RFootStepTime = 0.0;
		pGameInstance->StopSound(CSound_Manager::PLAYER_FOOTSTEPR);
		switch (rand() % 9 + 1) {
		case 1: {
			pGameInstance->SoundPlay(L"Walk1.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		case 2: {
			pGameInstance->SoundPlay(L"Walk2.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		case 3: {
			pGameInstance->SoundPlay(L"Walk3.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		case 4: {
			pGameInstance->SoundPlay(L"Walk4.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		case 5: {
			pGameInstance->SoundPlay(L"Walk5.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		case 6: {
			pGameInstance->SoundPlay(L"Walk6.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		case 7: {
			pGameInstance->SoundPlay(L"Walk7.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		case 8: {
			pGameInstance->SoundPlay(L"Walk8.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		case 9: {
			pGameInstance->SoundPlay(L"Walk9.ogg", CSound_Manager::PLAYER_FOOTSTEPR, SOUND_THREE);
		}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::VopalSound(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->StopSound(CSound_Manager::PLAYER_WEAPON);
	switch (rand() % 7 + 1) {
	case 1: {
		pGameInstance->SoundPlay(L"Vorpal_Slash01.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_WEIGHT);
	}
	case 2: {
		pGameInstance->SoundPlay(L"Vorpal_Slash02.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_WEIGHT);
	}
	case 3: {
		pGameInstance->SoundPlay(L"Vorpal_Slash03.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_WEIGHT);
	}
	case 4: {
		pGameInstance->SoundPlay(L"Vorpal_Slash04.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_WEIGHT);
	}
	case 5: {
		pGameInstance->SoundPlay(L"Vorpal_Slash05.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_WEIGHT);
	}
	case 6: {
		pGameInstance->SoundPlay(L"Vorpal_Slash06.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_WEIGHT);
	}
	case 7: {
		pGameInstance->SoundPlay(L"Vorpal_Slash07.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_WEIGHT);
	}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::HiccupSound(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_HiccupTime += TimeDelta;

	if (m_HiccupTime > 3.0) {
		m_HiccupTime = 0.0;
		pGameInstance->StopSound(CSound_Manager::PLAYER_BREATH);
		switch (rand() % 3 + 1) {
		case 1: {
			pGameInstance->SoundPlay(L"Alice_Hiccup1.ogg", CSound_Manager::PLAYER_BREATH, SOUND_THREE);
		}
		case 2: {
			pGameInstance->SoundPlay(L"Alice_Hiccup2.ogg", CSound_Manager::PLAYER_BREATH, SOUND_THREE);
		}
		case 3: {
			pGameInstance->SoundPlay(L"Alice_Hiccup3.ogg", CSound_Manager::PLAYER_BREATH, SOUND_THREE);
		}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::DamageSound(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->StopSound(CSound_Manager::PLAYER_BREATH);
	switch (rand() % 4 + 1) {
	case 1: {
		pGameInstance->SoundPlay(L"Alice_Damage1.ogg", CSound_Manager::PLAYER_BREATH, SOUND_DEFAULT);
	}
	case 2: {
		pGameInstance->SoundPlay(L"Alice_Damage2.ogg", CSound_Manager::PLAYER_BREATH, SOUND_DEFAULT);
	}
	case 3: {
		pGameInstance->SoundPlay(L"Alice_Damage3.ogg", CSound_Manager::PLAYER_BREATH, SOUND_DEFAULT);
	}
	case 4: {
		pGameInstance->SoundPlay(L"Alice_Damage4.ogg", CSound_Manager::PLAYER_BREATH, SOUND_DEFAULT);
	}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::GunFireSouund(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->StopSound(CSound_Manager::PLAYER_WEAPON);
	switch (rand() % 6 + 1) {
	case 1: {
		pGameInstance->SoundPlay(L"Pepperg_Fire01.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_DEFAULT);
	}
	case 2: {
		pGameInstance->SoundPlay(L"Pepperg_Fire02.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_DEFAULT);
	}
	case 3: {
		pGameInstance->SoundPlay(L"Pepperg_Fire03.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_DEFAULT);
	}
	case 4: {
		pGameInstance->SoundPlay(L"Pepperg_Fire04.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_DEFAULT);
	}
	case 5: {
		pGameInstance->SoundPlay(L"Pepperg_Fire05.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_DEFAULT);
	}
	case 6: {
		pGameInstance->SoundPlay(L"Pepperg_Fire06.ogg", CSound_Manager::PLAYER_WEAPON, SOUND_DEFAULT);
	}
	}
	RELEASE_INSTANCE(CGameInstance);

}

CPlayer * CPlayer::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CPlayer*	pInstance = new CPlayer(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();	
	//Safe_Release(m_pSphereCom);
	//Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
