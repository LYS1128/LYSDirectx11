#include "stdafx.h"
#include "..\Public\Shell.h"
#include "GameInstance.h"

CShell::CShell(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CShell::CShell(const CShell & rhs)
	: CGameObject(rhs)
{
}

HRESULT CShell::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShell::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	
	_vector Pos = *((_vector*)pArg);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);

	return S_OK;
}

void CShell::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CShell::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*		pTargetAABB = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Weapon"), TEXT("Com_AABB"));
	if (nullptr == pTargetAABB)
		return;

	if (pTargetAABB->Get_Active()) {
		if (m_pAABBCom->Collsion_AABB(pTargetAABB)) {
			m_eState = STATE_DEAD;
			DeathSound(TimeDelta);
			for (_uint i = 0; i <3; ++i) {
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_Spark"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
					return;
				}
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartA"),&m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartB"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartC"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
		}
			
	}

	list<CGameObject*>* pBulletList = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Bullet");

	if (pBulletList != nullptr) {
		for (auto iter : *pBulletList) {
			if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				m_eState = STATE_DEAD;
				DeathSound(TimeDelta);
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartA"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
					return;
				}
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartB"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
					return;
				}
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartC"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
					return;
				}
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
					return;
				}
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
					return;
				}
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
					return;
				}
				break;
			}

		}

	}

	CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());


	if (pPlayer->Get_PlayerMode() == 3) {

		pTargetAABB = (CCollider*)(pPlayer->Get_Component(L"Com_AABB"));

		if (nullptr == pTargetAABB)
			return;
		if (m_pAABBCom->Collsion_AABB(pTargetAABB)) {
			m_eState = STATE_DEAD;
			DeathSound(TimeDelta);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartA"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartB"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_ShellPartC"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Tooth", TEXT("Prototype_GameObject_Tooth"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
				MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
				return;
			}
		}

	}
		
	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
	{
	/*		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);*/
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CShell::Render()
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

HRESULT CShell::Render_LightDepth()
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

HRESULT CShell::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Shell"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.65f, 0.f);
	ColliderDesc.vSize = _float3(1.f, 1.3f, 1.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CShell::SetUp_ConstantTable()
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

void CShell::DeathSound(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->StopSound(CSound_Manager::SHELL_EFFECT);
	switch (rand() % 3 + 1) {
	case 1: {
		pGameInstance->SoundPlay(L"Shell1.ogg", CSound_Manager::SHELL_EFFECT, SOUND_DEFAULT);
	}
	case 2: {
		pGameInstance->SoundPlay(L"Shell2.ogg", CSound_Manager::SHELL_EFFECT, SOUND_DEFAULT);
	}
	case 3: {
		pGameInstance->SoundPlay(L"Shell3.ogg", CSound_Manager::SHELL_EFFECT, SOUND_DEFAULT);
	}

	}
	RELEASE_INSTANCE(CGameInstance);
}

CShell * CShell::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CShell*	pInstance = new CShell(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CShell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShell::Clone(void * pArg)
{
	CShell*	pInstance = new CShell(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CShell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShell::Free()
{
	__super::Free();	
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
