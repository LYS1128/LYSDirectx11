#include "stdafx.h"
#include "..\Public\Blade.h"
#include "GameInstance.h"

CBlade::CBlade(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CBlade::CBlade(const CBlade & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBlade::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlade::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_SocketDesc, pArg, sizeof(SOCKETDESC));

	m_pSocketMatrix = m_SocketDesc.pModelCom->Get_CombinedTransformationMatrix(m_SocketDesc.pBoneName);

	m_PivotMatrix = m_SocketDesc.pModelCom->Get_PivotMatrix();

	if (nullptr == m_pSocketMatrix)
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	



	if (FAILED(SetUp_Components()))
		return E_FAIL;	
	

	//m_pTransformCom->Scaled(_float3(10.f, 10.f, 10.f));

	return S_OK;
}

void CBlade::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	list<CGameObject*>* pPlayerList = pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player");

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pPlayerList->front());
	CTransform* pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));

	if (m_bAABB == false && pPlayer->Get_SwardAABB() == true) {
		m_bAABB = true;
		m_pAABBCom->Set_Active(true);
	}

	if (m_bAABB == true && pPlayer->Get_SwardAABB() == false) {
		m_bAABB = false;
		m_pAABBCom->Set_Active(false);
	}

	_matrix AABBMat = pPlayerTrans->Get_WorldMatrix();
	AABBMat.r[3] += 0.3* pPlayerTrans->Get_State(CTransform::STATE_LOOK);
	m_pAABBCom->Update(AABBMat);

	RELEASE_INSTANCE(CGameInstance);

}

void CBlade::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());
	if ((pPlayer->Get_Attach() == false && pPlayer->Get_PlayerMode() == 0 && pPlayer->Get_Rush() == false && pPlayer->Get_DropDead() == false && pPlayer->Get_Jump() == false) && pPlayer->Get_SuperStart() == false) {
		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBlade::Render()
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
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 1)))
			return E_FAIL;
	}	
//#ifdef _DEBUG
//	if (m_bAABB == true && m_pAABBCom != nullptr)
//		m_pAABBCom->Render();
//#endif // _DEBUG
	return S_OK;
}

HRESULT CBlade::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Blade"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	///* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f,0.825f,0.f);
	ColliderDesc.vSize = _float3(1.f, 1.65f, 1.f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CBlade::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CTransform* pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));
	/* 뼈의 스케일상태는 무시. 회전값과 이동값만 무기의 상태로 사용한다. */
	_matrix		BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_matrix PlayerWorldMat = pPlayerTrans->Get_WorldMatrix();

	XMStoreFloat4x4(&m_pWorldMat, XMMatrixTranspose(BoneMatrix * XMLoadFloat4x4(&m_PivotMatrix)*PlayerWorldMat));

	m_pShaderCom->Set_RawValue("g_SocketMatrix", &m_pWorldMat, sizeof(_float4x4));
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CBlade * CBlade::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CBlade*	pInstance = new CBlade(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBlade"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlade::Clone(void * pArg)
{
	CBlade*	pInstance = new CBlade(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBlade"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlade::Free()
{
	__super::Free();	
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
