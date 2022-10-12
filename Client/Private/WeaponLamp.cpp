#include "stdafx.h"
#include "..\Public\WeaponLamp.h"
#include "GameInstance.h"

CWeaponLamp::CWeaponLamp(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CWeaponLamp::CWeaponLamp(const CWeaponLamp & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWeaponLamp::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponLamp::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	m_vPlayerPos = *((_vector*)pArg);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPlayerPos + XMVectorSet(0.f, 10.f, 0.f, 0.f));
	m_pAttAABBCom->Set_Active(true);
	return S_OK;
}

void CWeaponLamp::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) >= XMVectorGetY(m_vPlayerPos)) {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) - XMVectorSet(0.f,0.5f,0.f,0.f));
	}
	else if (m_iSpawn == false) {
		m_iSpawn = true;
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		pGameInstance->StopSound(CSound_Manager::SCENE_SOUND2);
		pGameInstance->SoundPlay(L"CardGuard_WeaponDrop.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
		RELEASE_INSTANCE(CGameInstance);

	}
	else {
		m_pAttAABBCom->Set_Active(false);
		Time += TimeDelta;
		if (Time > 1.f)
			m_eState = STATE_DEAD;
	}

	m_pAttAABBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CWeaponLamp::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

}

HRESULT CWeaponLamp::Render()
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

#ifdef _DEBUG
	m_pAttAABBCom->Render();
	//m_pOBBCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CWeaponLamp::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FleshLamp"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 3.5f, 0.f);
	ColliderDesc.vSize = _float3(1.f, 7.f, 1.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AttAABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAttAABBCom, &ColliderDesc)))
		return E_FAIL;


	/* For.Com_OBB */
	//ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	//ColliderDesc.vSize = _float3(0.9f, 1.0f, 0.9f);

	//if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CWeaponLamp::SetUp_ConstantTable()
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

	/* For.LightDesc */
	/*const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;	
*/
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CWeaponLamp * CWeaponLamp::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CWeaponLamp*	pInstance = new CWeaponLamp(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CWeaponLamp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWeaponLamp::Clone(void * pArg)
{
	CWeaponLamp*	pInstance = new CWeaponLamp(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CWeaponLamp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeaponLamp::Free()
{
	__super::Free();	
	//Safe_Release(m_pOBBCom);
	Safe_Release(m_pAttAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
