#include "stdafx.h"
#include "..\Public\JumpPad.h"
#include "GameInstance.h"

CJumpPad::CJumpPad(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CJumpPad::CJumpPad(const CJumpPad & rhs)
	: CGameObject(rhs)
{
}

HRESULT CJumpPad::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CJumpPad::NativeConstruct(void * pArg)
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
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(24.3f, 30.1f, 128.0f, 1.f));
	m_pAABBCom->Set_Active(true);

	return S_OK;
}

void CJumpPad::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pModelCom->Set_AnimationIndex(m_iAnimationIndex);
	m_pModelCom->Update(TimeDelta);
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CJumpPad::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = (CPlayer*)pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front();
	CCollider*		pTargetAABB = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_AABB"));
	if (nullptr == pTargetAABB)
		return;
	if (m_pAABBCom->Collsion_AABB(pTargetAABB) && m_pModelCom->Get_ChangeEnd()) {
		pGameInstance->StopSound(CSound_Manager::PLAYER_EFFECT);
		pGameInstance->SoundPlay(L"SuperJump.ogg", CSound_Manager::PLAYER_EFFECT, SOUND_DEFAULT);
		pPlayer->Set_Jump(true);
		pPlayer->Set_SuperJump(true);
		m_iAnimationIndex = 2;
		m_pModelCom->Set_Cut(false);
		m_pModelCom->Set_Speed(1.f);
		m_bLaunch = true;
	}
	else if (m_bLaunch == true && m_pModelCom->Get_Animation(2)->Get_KeyFrameENd()) {
		if (m_pModelCom->Get_ChangeEnd()) {
			pGameInstance->StopSound(CSound_Manager::PLAYER_EFFECT);
			m_iAnimationIndex = 0;
			m_pModelCom->Set_Cut(false);
			m_pModelCom->Set_Speed(1.f);
			m_bLaunch = false;
		}
	}
	//if (nullptr != m_pRendererCom)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_LIGHTDEPTH, this);
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CJumpPad::Render()
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

HRESULT CJumpPad::Render_LightDepth()
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

HRESULT CJumpPad::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_JumpPad"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.25f, 0.f);
	ColliderDesc.vSize = _float3(1.5f, 0.5f, 1.5f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;


	/* For.Com_OBB */
	//ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	//ColliderDesc.vSize = _float3(0.9f, 1.0f, 0.9f);

	//if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CJumpPad::SetUp_ConstantTable()
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

CJumpPad * CJumpPad::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CJumpPad*	pInstance = new CJumpPad(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CJumpPad"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CJumpPad::Clone(void * pArg)
{
	CJumpPad*	pInstance = new CJumpPad(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CJumpPad"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CJumpPad::Free()
{
	__super::Free();	
	//Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
