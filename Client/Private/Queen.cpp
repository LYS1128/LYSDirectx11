#include "stdafx.h"
#include "..\Public\Queen.h"
#include "GameInstance.h"

CQueen::CQueen(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CQueen::CQueen(const CQueen & rhs)
	: CGameObject(rhs)
{
}

HRESULT CQueen::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQueen::NativeConstruct(void * pArg)
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

	m_pModelCom->Set_PreAnimIndex(0);
	m_pModelCom->Set_AnimationIndex(0);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.0f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.1f, 130.82f, 1.f));
	m_pAABBCom->Set_Active(true);
	m_pAttAABBCom->Set_Active(false);
	m_pModelCom->Set_Cut(true);
	return S_OK;
}

void CQueen::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_iAnimState >= 3) {
		m_fSquareTime += TimeDelta;
		if (m_fSquareTime >= 5.f) {
			m_fSquareTime = 0.f;
			_uint i = rand() % 5;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Effect", TEXT("Prototype_GameObject_RedSquare"), &i))) {
				MSG_BOX(L"Failed To CLevel_Boss : Boss, Queen");
				return;
			}
		}
	}

	if (m_iAnimState == 1) {
		m_bSound = true;
		if (m_pModelCom->Get_Animation(m_iAnimationIndex)->Get_CurrnetKeyFrame() == 200)
			m_iAnimState = 2;
	}
	else if (m_iAnimState == 2) {
		if (m_pModelCom->Get_Animation(m_iAnimationIndex)->Get_CurrnetKeyFrame() == 234 && m_bTower == false) {
			m_bTower = true;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_QueenTower", TEXT("Prototype_GameObject_QueenTower"), &XMVectorSet(-18.f, 0.f, 145.f, 0.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_QueenTower", TEXT("Prototype_GameObject_QueenTower"), &XMVectorSet(-23.f, 0.f, 138.f, 1.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_QueenTower", TEXT("Prototype_GameObject_QueenTower"), &XMVectorSet(23.f, 0.f, 138.f, 2.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_QueenTower", TEXT("Prototype_GameObject_QueenTower"), &XMVectorSet(18.f, 0.f, 145.f, 3.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
		}
		if (m_pModelCom->Get_Animation(m_iAnimationIndex)->Get_CurrnetKeyFrame() == 272) {
			m_iAnimState = 3;
			m_iAnimationIndex = 0;
			m_pModelCom->Set_Cut(false);
		
		}
	}
	else if (m_iAnimState == 4 ) {
		if (m_bDoll == false) {

			dynamic_cast<CQueenTower*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenTower")->front())->Set_Dead(true);

			m_bDoll = true;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Doll", TEXT("Prototype_GameObject_Doll"), &XMVectorSet(-23.837f, 0.f, 69.267f, 40.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Doll", TEXT("Prototype_GameObject_Doll"), &XMVectorSet(-31.467f, 0.f, 99.281f, 52.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Doll", TEXT("Prototype_GameObject_Doll"), &XMVectorSet(24.213f, 0.f, 120.296f, 63.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Doll", TEXT("Prototype_GameObject_Doll"), &XMVectorSet(18.311f, 0.f, 62.853f, 39.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Heart", TEXT("Prototype_GameObject_Heart")))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
		}
		else {
			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Heart")->size() == 0 && pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Doll")->size() != 0) {

				m_HeartSpawn += TimeDelta;

				if (m_HeartSpawn >= 3.f) {
					m_HeartSpawn = 0.f;
					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Heart", TEXT("Prototype_GameObject_Heart")))) {
						MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
						return;
					}
				}
			}
			
		}
	}
	else if (m_iAnimState == 5) {
		

		if (m_bKnight == false) {
			dynamic_cast<CQueenTower*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenTower")->front())->Set_Dead(true);

			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Heart")->size() != 0)
				pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Heart")->front()->Set_Dead();
			m_bKnight = true;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Knight", TEXT("Prototype_GameObject_Knight"), &XMVectorSet(-23.837f, 0.f, 69.267f, 40.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Knight", TEXT("Prototype_GameObject_Knight"), &XMVectorSet(-31.467f, 0.f, 99.281f, 52.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Knight", TEXT("Prototype_GameObject_Knight"), &XMVectorSet(24.213f, 0.f, 120.296f, 63.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Knight", TEXT("Prototype_GameObject_Knight"), &XMVectorSet(18.311f, 0.f, 62.853f, 39.f)))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
		}
	}
	else if (m_iAnimState == 6) {

		if (m_bCircle == false) {
			m_bCircle = true;
			dynamic_cast<CQueenTower*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenTower")->front())->Set_Dead(true);

			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->size() != 0) {
				pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->front()->Set_Dead();
				dynamic_cast<CPlayer*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front())->Set_ShaderPath(0);
			}
		}

		m_fChandTIme += TimeDelta;
		
		if (m_fChandTIme >= 5.f) {
			m_fChandTIme = 0.f;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Effect", TEXT("Prototype_GameObject_RedCircle")))) {
				MSG_BOX(L"Failed To CLevel_Boss : Boss, Queen");
				return;
			}
		}
	
	}
	else if (m_iAnimState == 7) {
		if (m_bDead == false) {
			m_bDead = true;
			pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
			pGameInstance->SoundPlay(L"Queen_Death.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
			dynamic_cast<CQueenTower*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenTower")->front())->Set_Dead(true);
		}
		++m_iDeadcount;
		m_pPlayer->Set_PlayerMode(4);
		m_iAnimationIndex = 6;
		m_pModelCom->Set_Cut(true);

		if (m_pModelCom->Get_Animation(6)->Get_KeyFrameENd()) {
			m_eState = STATE_DEAD;
			m_bSound = false;
			pGameInstance->StopSound(CSound_Manager::BGM);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_QueenMace", TEXT("Prototype_GameObject_QueenMace")))) {
				MSG_BOX(L"Failed To CLevel_Boss : Boss, Queen");
				return;
			}
			pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenBase")->front()->Set_Dead();

		}
	}

	_matrix AABBMat = m_pTransformCom->Get_WorldMatrix();
	AABBMat.r[3] += 0.3* m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	m_pAttAABBCom->Update(AABBMat);
	RELEASE_INSTANCE(CGameInstance);

	m_pModelCom->Set_AnimationIndex(m_iAnimationIndex);
	m_pModelCom->Update(TimeDelta);
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	//m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CQueen::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//list<CGameObject*>* pBulletList = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Bullet");

	//if (pBulletList != nullptr) {
	//	for (auto iter : *pBulletList) {
	//		if (m_iHp > 0 && m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
	//			--m_iHp;
	//			}
	//		}
	//	}

	//RELEASE_INSTANCE(CGameInstance);
	//if (nullptr != m_pRendererCom)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);


}

HRESULT CQueen::Render_LightDepth()
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

HRESULT CQueen::Render()
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

HRESULT CQueen::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_QueenBody"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	ColliderDesc.vSize = _float3(1.2f, 1.4f, 1.2f);

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

HRESULT CQueen::SetUp_ConstantTable()
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

CQueen * CQueen::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CQueen*	pInstance = new CQueen(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CQueen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQueen::Clone(void * pArg)
{
	CQueen*	pInstance = new CQueen(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CQueen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQueen::Free()
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
