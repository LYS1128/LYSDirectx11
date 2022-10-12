  #  include "stdafx.h"
#include "..\Public\Doll.h"
#include "GameInstance.h"

CDoll::CDoll(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CDoll::CDoll(const CDoll & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDoll::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoll::NativeConstruct(void * pArg)
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
	
	m_iCurrentCellIndex = XMVectorGetW(Arg);
	_vector vPos = XMVectorSetW(Arg, 1.f);
	m_vPrePos = vPos;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Navigationheight(&m_iCurrentCellIndex);

	m_pModelCom->Set_PreAnimIndex(0);
	m_pModelCom->Set_AnimationIndex(0);
	m_pAABBCom->Set_Active(true);
	return S_OK;
}

void CDoll::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_iHP <= 0) {
		m_eState = STATE_DEAD;
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_DollHead"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
			return;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_DollLeft"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
			return;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_DollRight"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
			return;
		}
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Effect", TEXT("Prototype_GameObject_DollBody"), &m_pTransformCom->Get_State(CTransform::STATE_POSITION)))) {
			MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
			return;
		}
		RELEASE_INSTANCE(CGameInstance);
	}
	if (m_bTarget == false) {
		Time += TimeDelta;

		if (Time > 3.14f) {

			_float3 PrePos;
			XMStoreFloat3(&PrePos, m_vPrePos);
			_float3 MyPos;
			XMStoreFloat3(&MyPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			_float Dist = sqrtf((PrePos.x - MyPos.x)*(PrePos.x - MyPos.x) + (PrePos.y - MyPos.y)*(PrePos.y - MyPos.y) + (PrePos.z - MyPos.z)*(PrePos.z - MyPos.z));

			if (Dist <= 1.f) {
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(Rand % 360));
				m_iStuckTime = 0;
			}

			m_vPrePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			Rand = rand();
			Time = 0.f;
		}

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), pow(-1, Rand % 2 + 1) * 0.1f*TimeDelta);
		m_pTransformCom->Go_StraightOnNavigation(Rand % 3 * 0.5f*TimeDelta + 0.005f, &m_iCurrentCellIndex, false);
	}
	else {
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		CTransform* pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));
		
		RELEASE_INSTANCE(CGameInstance);

		_float3 PlayerPos;
		XMStoreFloat3(&PlayerPos, pPlayerTrans->Get_State(CTransform::STATE_POSITION));
		_float3 MyPos;
		XMStoreFloat3(&MyPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_float Dist = sqrtf((PlayerPos.x - MyPos.x)*(PlayerPos.x - MyPos.x) + (PlayerPos.y - MyPos.y)*(PlayerPos.y - MyPos.y) + (PlayerPos.z - MyPos.z)*(PlayerPos.z - MyPos.z));

		_vector Go = XMVectorSet(PlayerPos.x, 0.f, PlayerPos.z, 0.f) - XMVectorSet(MyPos.x, 0.f, MyPos.z, 0.f);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(Go));

		_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

		_vector		vUp = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

		if(Dist >= 0.5f)
			m_pTransformCom->Go_StraightOnNavigation(TimeDelta, &m_iCurrentCellIndex, false);
	}

	m_pModelCom->Set_AnimationIndex(m_iAnimationIndex);
	m_pModelCom->Update(TimeDelta);
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CDoll::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	list<CGameObject*>* pObjectList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Doll");

	if (pObjectList != nullptr) {
		for (auto iter : *pObjectList) {
			if (iter == this)
				continue;

			if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				_float3 MySize = _float3(1.2f, 4.f, 1.2f);
				_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
				_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
				m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
			}
		}
	}

	list<CGameObject*>* pTowerList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_QueenTower");

	if (pTowerList != nullptr) {
		for (auto iter : *pTowerList) {
			if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
				_float3 MySize = _float3(1.2f, 4.f, 1.2f);
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
				_float3 MySize = _float3(1.2f, 4.f, 1.2f);
				_float3 ObjSize = dynamic_cast<CCollider*>(iter->Get_Component(L"Com_AABB"))->Get_Size();
				_vector ObjectPos = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
				m_pTransformCom->Collide(MySize, ObjSize, ObjectPos);
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

}

HRESULT CDoll::Render()
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
//	//m_pOBBCom->Render();
//#endif // _DEBUG
	return S_OK;
}

HRESULT CDoll::Render_LightDepth()
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

HRESULT CDoll::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_Doll"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	ColliderDesc.vSize = _float3(1.2f, 4.f, 1.2f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;


	
	return S_OK;
}

HRESULT CDoll::SetUp_ConstantTable()
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

CDoll * CDoll::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CDoll*	pInstance = new CDoll(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CDoll"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDoll::Clone(void * pArg)
{
	CDoll*	pInstance = new CDoll(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CDoll"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoll::Free()
{
	__super::Free();
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
