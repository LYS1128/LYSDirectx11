#include "stdafx.h"
#include "..\Public\Knight.h"
#include "GameInstance.h"

CKnight::CKnight(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CKnight::CKnight(const CKnight & rhs)
	: CGameObject(rhs)
{
}

HRESULT CKnight::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKnight::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	_vector Arg = *((_vector*)pArg);
	m_iIndex = XMVectorGetW(Arg);

	if (m_iIndex % 2 == 0)
		m_bTypeRed = true;
	else
		m_bTypeRed = false;


	m_vPos = XMVectorSetW(Arg, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0.f, 0.1f, 0.f, 0.f));

	if (FAILED(SetUp_Components()))
		return E_FAIL;	



	return S_OK;
}

void CKnight::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	Time += TimeDelta;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));
	CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());

	_float3 PlayerPos;
	XMStoreFloat3(&PlayerPos, pPlayerTrans->Get_State(CTransform::STATE_POSITION));
	_float3 MyPos;
	XMStoreFloat3(&MyPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	_float Dist = sqrtf((PlayerPos.x - MyPos.x)*(PlayerPos.x - MyPos.x) + (PlayerPos.z - MyPos.z)*(PlayerPos.z - MyPos.z));

	if (m_iHP <= 0) {
		m_eState = STATE_DEAD;
		if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado") != nullptr && pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->size() > 0) {
			pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->front()->Set_Dead();
			dynamic_cast<CPlayer*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front())->Set_ShaderPath(0);
		}

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Tornado", TEXT("Prototype_GameObject_Tornado"), &m_bTypeRed))) {
			MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
			return;
		}
	}

	if (Dist <= 5.f) {
		if (m_bPlayerIN == false) {
			m_bPlayerIN = true;
			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado") != nullptr && pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->size() > 0) {
				if (dynamic_cast<CTornado*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->front())->Get_Red() == m_bTypeRed) {
					pPlayer->Set_Hp(0);
					pPlayer->Set_ShaderPath(0);
				}
			}
		}
		if (Time >= 3.f && pPlayer->Get_Super() == false) {
			pPlayer->Set_Damaged(5);
			Time = 0.f;
		}
	}
	else{
		if (m_bPlayerIN == true) {
			m_bPlayerIN = false;
			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado") != nullptr && pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->size() > 0) {
				pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Tornado")->front()->Set_Dead();
				dynamic_cast<CPlayer*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front())->Set_ShaderPath(0);
			}

			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Tornado", TEXT("Prototype_GameObject_Tornado"), &m_bTypeRed))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CKnight::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = (CPlayer*)pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front();
	CCollider*		pTargetAABB = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Weapon"), TEXT("Com_AABB"));
	if (nullptr == pTargetAABB)
		return;

	if (pTargetAABB->Get_Active()) {
		if (m_pAABBCom->Collsion_AABB(pTargetAABB)) {
			if (m_iHP >= 1) {
				if (pPlayer->Get_Super() && pPlayer->Get_Hp() < 79)
					pPlayer->Set_Damaged(-1);
				--m_iHP;
				HitSound();
				for (_uint i = 0; i <3; ++i) {
					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_Spark"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
						return;
					}
				}
			}
		}		
	}
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CKnight::Render()
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
//#endif // _DEBUG

	return S_OK;
}

HRESULT CKnight::Render_LightDepth()
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

HRESULT CKnight::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (m_bTypeRed) {
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_RedKnight"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
	}
	else {
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_WhiteKnight"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
	}

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 1.5f, 0.f);
	ColliderDesc.vSize = _float3(1.5f, 3.f, 1.5f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CKnight::SetUp_ConstantTable()
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

void CKnight::HitSound()
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

CKnight * CKnight::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CKnight*	pInstance = new CKnight(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CKnight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CKnight::Clone(void * pArg)
{
	CKnight*	pInstance = new CKnight(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CKnight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKnight::Free()
{
	__super::Free();	
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
