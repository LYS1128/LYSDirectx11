#include "stdafx.h"
#include "..\Public\Heart.h"
#include "GameInstance.h"

CHeart::CHeart(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CHeart::CHeart(const CHeart & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHeart::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHeart::NativeConstruct(void * pArg)
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
	pGameInstance->StopSound(CSound_Manager::TOOTH_EFFECT);
	RELEASE_INSTANCE(CGameInstance);
	m_vPos = m_pPlayerTrans->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f);
	m_pModelCom->Set_PreAnimIndex(2);
	m_pModelCom->Set_AnimationIndex(2);
	m_pModelCom->Set_Cut(false);
	m_pAABBCom->Set_Active(true);

	return S_OK;
}

void CHeart::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->SoundPlay(L"HeartBeat.ogg", CSound_Manager::TOOTH_EFFECT, SOUND_THREE);

	m_fCoolTIme += TimeDelta;
	if (m_bDoll == false) {
		m_vPos = m_pPlayerTrans->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0.f, 2.f, 0.f, 0.f));
	}
	else
	{
		CTransform* DollTrans = (CTransform*)(m_pTargetDoll->Get_Component(L"Com_Transform"));
		m_vPos = DollTrans->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos + XMVectorSet(0.f, 4.f, 0.f, 0.f));
	}

	

	if (m_pModelCom->Get_Animation(m_iAnimationIndex)->Get_KeyFrameENd())
		++m_iTime;
	
	if (m_iTime >= 10) {
		if (m_bDoll) {
			m_pTargetDoll->Set_HP_Zero();
			
			pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
			pGameInstance->SoundPlay(L"Doll_Death.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
			
		}
		else
		{
			m_pPlayer->Set_Hp(0);
		}
		m_eState = STATE_DEAD;
		pGameInstance->StopSound(CSound_Manager::SHELL_EFFECT);
		pGameInstance->SoundPlay(L"Heart_Death.ogg", CSound_Manager::SHELL_EFFECT, SOUND_DEFAULT);
	}
	RELEASE_INSTANCE(CGameInstance);
	m_pModelCom->Set_AnimationIndex(m_iAnimationIndex);
	m_pModelCom->Update(TimeDelta);
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CHeart::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	list<CGameObject*>* pDollList = pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Doll");
	CPlayer* pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());

	if (m_fCoolTIme >= 3.f) {
		if (m_bDoll == false) {
			if (pDollList != nullptr && m_pAABBCom->Get_Active()) {
				for (auto iter : *pDollList) {
					if (m_pAABBCom->Collsion_AABB((CCollider*)(iter->Get_Component(L"Com_AABB")))) {
						if (dynamic_cast<CDoll*>(iter)->Get_Target() == false) {
							pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
							pGameInstance->SoundPlay(L"Doll_Touch1.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
							m_bDoll = true;
							m_pTargetDoll = (CDoll*)(iter);
							m_pTargetDoll->Set_Target(true);
							CTransform* DollTrans = (CTransform*)(iter->Get_Component(L"Com_Transform"));
							m_vPos = DollTrans->Get_State(CTransform::STATE_POSITION);
							m_fCoolTIme = 0;
							break;
						}
					}

				}

			}
		}
		else {
			if (m_pAABBCom->Collsion_AABB((CCollider*)(pPlayer->Get_Component(L"Com_AABB")))) {
				pGameInstance->StopSound(CSound_Manager::MONSTER_EFFECT);
				pGameInstance->SoundPlay(L"Doll_Touch2.ogg", CSound_Manager::MONSTER_EFFECT, SOUND_DEFAULT);
				m_bDoll = false;
				m_pTargetDoll->Set_Target(false);
				
				m_pTargetDoll = nullptr;
				CTransform* PlayerTrans = (CTransform*)(pPlayer->Get_Component(L"Com_Transform"));
				m_vPos = PlayerTrans->Get_State(CTransform::STATE_POSITION);
				m_fCoolTIme = 0;
			}
		}
	}
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CHeart::Render()
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
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 5)))
			return E_FAIL;
	}	
//
//#ifdef _DEBUG
//	m_pAABBCom->Render();
//	//m_pOBBCom->Render();
//#endif // _DEBUG

	return S_OK;
}

HRESULT CHeart::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_BOSS, TEXT("Prototype_Component_Model_Heart"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */

	ColliderDesc.vPosition = _float3(0.f, -2.5f, 0.f);
	ColliderDesc.vSize = _float3(0.5f, 5.f, 0.5f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CHeart::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	if (FAILED(m_pShaderCom->Set_RawValue("g_Count", &m_iTime, sizeof(_uint))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CHeart * CHeart::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CHeart*	pInstance = new CHeart(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CHeart"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHeart::Clone(void * pArg)
{
	CHeart*	pInstance = new CHeart(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CHeart"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHeart::Free()
{
	__super::Free();	
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
