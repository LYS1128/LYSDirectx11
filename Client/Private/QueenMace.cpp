#include "stdafx.h"
#include "..\Public\QueenMace.h"
#include "GameInstance.h"

CQueenMace::CQueenMace(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CQueenMace::CQueenMace(const CQueenMace & rhs)
	: CGameObject(rhs)
{
}

HRESULT CQueenMace::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQueenMace::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 1.f, 130.82f, 1.f));

	return S_OK;
}

void CQueenMace::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	
	if (m_bFadeOut) {
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		if (dynamic_cast<CFadeInOut*>(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Fade")->front())->Get_Alpha() == 1.f) {
			CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
			pPlayer->Set_PlayerMode(4);
			pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Fade")->front()->Set_Dead();
			_float2 f = _float2(0.f, 0.f);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Fade", TEXT("Prototype_GameObject_FadeInOut"), &f))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_QueenAlice", TEXT("Prototype_GameObject_QueenAlice")))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
			m_eState = STATE_DEAD;
		}
		RELEASE_INSTANCE(CGameInstance);
	}

	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CQueenMace::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*		pTargetAABB = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_AABB"));
	if (nullptr == pTargetAABB)
		return;

	if (pTargetAABB->Get_Active()) {
		if (m_pAABBCom->Collsion_AABB(pTargetAABB) && m_bFadeOut == false) {
			m_bFadeOut = true;
			CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
			_float2 f = _float2(0.f,1.f);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Fade", TEXT("Prototype_GameObject_FadeInOut"), &f))) {
				MSG_BOX(L"Failed To CLevel_Boss : Ready_Layer_BackGround, TestMap");
				return;
			}
		}
	}
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CQueenMace::Render_LightDepth()
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

HRESULT CQueenMace::Render()
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

HRESULT CQueenMace::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_QueenMace"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 1.5f, 0.f);
	ColliderDesc.vSize = _float3(1.f, 5.f, 1.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CQueenMace::SetUp_ConstantTable()
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

CQueenMace * CQueenMace::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CQueenMace*	pInstance = new CQueenMace(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CQueenMace"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQueenMace::Clone(void * pArg)
{
	CQueenMace*	pInstance = new CQueenMace(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CQueenMace"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQueenMace::Free()
{
	__super::Free();	
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
