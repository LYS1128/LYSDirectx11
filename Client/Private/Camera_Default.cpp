#include "stdafx.h"
#include "..\Public\Camera_Default.h"
#include "GameInstance.h"

CCamera_Default::CCamera_Default(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CCamera(pDeviceOut, pDeviceContextOut)
{
}

CCamera_Default::CCamera_Default(const CCamera_Default & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Default::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Default::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 10.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayer = (CPlayer*)(pGameInstance->Find_Layer_List(LEVEL_STATIC, L"Layer_Player")->front());


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CCamera_Default::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	
	Safe_AddRef(pGameInstance);
	CTransform* pTargetTransform = (CTransform*)(m_pPlayer->Get_Component(L"Com_Transform"));
	Safe_AddRef(pTargetTransform);

	_float x = m_fRad*sin(XMConvertToRadians(m_fTheta))*cos(XMConvertToRadians(m_fPhi));
	_float y = m_fRad*cos(XMConvertToRadians(m_fTheta));
	_float z =  m_fRad*sin(XMConvertToRadians(m_fTheta))*sin(XMConvertToRadians(m_fPhi));

	_long		MouseMove;

	if (m_pPlayer->Get_PlayerMode() != 1 && m_pPlayer->Get_PlayerMode() !=4) {

		if (m_pPlayer->Get_PlayerMode() == 2)
			m_fRad = 4.f;
		else
			m_fRad = 3.f;

		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
		{
			m_fPhi += -TimeDelta * MouseMove*50.f;
		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
		{
			m_fTheta += -TimeDelta * MouseMove*50.f;
			if (m_fTheta <= 0.f)
				m_fTheta = 0.01f;
			if (m_fTheta >= 180.f)
				m_fTheta = 179.99f;
		}

		XMStoreFloat3(&m_CameraDesc.vEye, (pTargetTransform->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 1.f, 0.f, 0.f) + XMVectorSet(x, y, z, 0.f)));
		XMStoreFloat3(&m_CameraDesc.vAt, (pTargetTransform->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	}
	else if (m_pPlayer->Get_PlayerMode() == 1){

		XMStoreFloat3(&m_CameraDesc.vEye, (pTargetTransform->Get_State(CTransform::STATE_POSITION) - 0.8f*pTargetTransform->Get_State(CTransform::STATE_LOOK) + 0.5f*pTargetTransform->Get_State(CTransform::STATE_RIGHT) + 1.3f*pTargetTransform->Get_State(CTransform::STATE_UP) ));
		XMStoreFloat3(&m_CameraDesc.vAt, (pTargetTransform->Get_State(CTransform::STATE_POSITION) + 5* pTargetTransform->Get_State(CTransform::STATE_LOOK) + 0.2f*pTargetTransform->Get_State(CTransform::STATE_RIGHT) + XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	}
	else if (m_pPlayer->Get_PlayerMode() == 4) {
		if (m_pPlayer->Get_SceneNum() == 1) {
			CTransform* pDominoTransform = (CTransform*)(pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Domino", L"Com_Transform"));
			XMStoreFloat3(&m_CameraDesc.vAt, pDominoTransform->Get_State(CTransform::STATE_POSITION));
		}
		else if (m_pPlayer->Get_SceneNum() == 2) {

			if (Stop == 0 && XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < 170.f) {
				_vector fGo = XMVector4Normalize(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - XMVectorSet(-145.935f, -24.0f, 176.283f, 1.f));
				XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(m_CameraDesc.vEye.x, m_CameraDesc.vEye.y, m_CameraDesc.vEye.z, 1.f) - 0.5f*fGo);
				XMStoreFloat3(&m_CameraDesc.vAt, XMVectorSet(-145.935f, -24.0f, 176.283f, 1.f));
			}
			else if (Stop == 0)
				Stop = 1;

			if (Stop == 1 && XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < 176.f) {
				XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(m_CameraDesc.vEye.x + 0.2f, m_CameraDesc.vEye.y, m_CameraDesc.vEye.z + 0.3f, 1.f));
			}
			else if (Stop == 1) {
				Stop = 2;
				CTransform* pPlayerTransform = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));
				Go = XMVector4Normalize(pPlayerTransform->Get_State(CTransform::STATE_POSITION) - XMVectorSet(m_CameraDesc.vAt.x, m_CameraDesc.vAt.y, m_CameraDesc.vAt.z, 1.f));				
			}
			
			if (Stop == 2 && Time <= 31) {
				++Time;
				XMStoreFloat3(&m_CameraDesc.vAt, XMVectorSet(m_CameraDesc.vAt.x, m_CameraDesc.vAt.y, m_CameraDesc.vAt.z, 1.f) + 0.8f*Go);
				XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(m_CameraDesc.vEye.x - 0.18f, m_CameraDesc.vEye.y, m_CameraDesc.vEye.z, 1.f));

			}
			else if (Stop == 2 && Time == 32) {
				++Time;
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Object", TEXT("Prototype_GameObject_MetalPole")))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
					return;
				}
				if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"Layer_Object", TEXT("Prototype_GameObject_FleshLamp")))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_BackGround, TestMap");
					return;
				}
			}
			else if (Stop == 2 && Time > 32 && Time <= 51) {
				++Time;
				XMStoreFloat3(&m_CameraDesc.vAt, XMVectorSet(m_CameraDesc.vAt.x, m_CameraDesc.vAt.y, m_CameraDesc.vAt.z, 1.f) + 0.8f*Go);
				XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(m_CameraDesc.vEye.x - 0.18f, m_CameraDesc.vEye.y, m_CameraDesc.vEye.z, 1.f));
			}
			
			else if (Stop == 2 && Time <= 90) {
				++Time;
			}
			else if (Stop == 2){
				Stop = 3;
				m_pPlayer->Set_PlayerMode(0);
				m_pPlayer->Set_Scene(3);
			}
		}
		else if (m_pPlayer->Get_SceneNum() == 3) {
			CTransform* pTransform = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_CarriageCar", L"Com_Transform"));
			XMStoreFloat3(&m_CameraDesc.vAt, pTransform->Get_State(CTransform::STATE_POSITION));
			XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(-145.935f, -5.985f, 176.283f, 1.f));
		}
		else if (m_pPlayer->Get_SceneNum() == 4) {
			if (Stop2 == 0) {
				CTransform* pTransform = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_CarriageCar", L"Com_Transform"));
				XMStoreFloat3(&m_CameraDesc.vAt, pTransform->Get_State(CTransform::STATE_POSITION)-XMVectorSet(0.f,2.f,0.f,0.f));
				XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(-139.127f, -11.389f, 155.925f, 1.f));
				Stop2 = 1;
			}
			else if (Stop2 == 1) {
				CTransform* pTransform = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_CarriageCar", L"Com_Transform"));
				if (pTransform != nullptr) {
					XMStoreFloat3(&m_CameraDesc.vAt, pTransform->Get_State(CTransform::STATE_POSITION) - XMVectorSet(0.f, 2.f, 0.f, 0.f));
					_vector Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
					XMStoreFloat3(&m_CameraDesc.vEye, Pos + XMVectorSet(0.f, 0.f, 0.01f, 0.f));
				}
			}
		}
		else if (m_pPlayer->Get_SceneNum() == 5) {
			if (Stop3 == 0) {
				XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(0.f, 0.f, 0.f, 1.f));
				XMStoreFloat3(&m_CameraDesc.vAt, XMVectorSet(0.f, 0.f, 1.f, 1.f));
				CQueen* pQueen = (CQueen*)(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->front());
				pQueen->Set_AnimIndex(7);
				pQueen->Set_AnimState(1);
				Stop3 = 1;
				Time = 0;
			}
			else if (Stop3 == 1) {
				++Time;
				XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(m_CameraDesc.vEye.x, powf(powf(8.5f, 1.f / 130.f), Time/2.f) , m_CameraDesc.vEye.z + 0.5f, 1.f));
				XMStoreFloat3(&m_CameraDesc.vAt, XMVectorSet(m_CameraDesc.vEye.x, m_CameraDesc.vEye.y, m_CameraDesc.vEye.z + 1.f,1.f));
			
				if (Time >= 260) {
					Time = 0;
					Stop3 = 2;
				}
			}
			else if (Stop3 == 2) {
				CQueen* pQueen = (CQueen*)(pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->front());
				if (pQueen->Get_AnimState() == 2) {
					++Time;
					XMStoreFloat3(&m_CameraDesc.vEye, XMVectorSet(m_CameraDesc.vEye.x, m_CameraDesc.vEye.y, m_CameraDesc.vEye.z - 0.08f, 1.f));
				}
				else if (pQueen->Get_AnimState() == 3) {
					Stop3 = 3;
					Time = 0;
				}
			}
			else if (Stop3 == 3) {
				++Time;

				if (Time > 180) {
					Stop3 = 4;
					m_pPlayer->Set_PlayerMode(0);
					m_pPlayer->Set_Scene(6);
					Time = 0;

					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Executioner", TEXT("Prototype_GameObject_Executioner")))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Monster, IronHive");
						return;
					}

					CBlade::SOCKETDESC			SocketDesc;
					ZeroMemory(&SocketDesc, sizeof(CBlade::SOCKETDESC));

					SocketDesc.pModelCom = (CModel*)pGameInstance->Get_Component(LEVEL_BOSS, L"Layer_Executioner", TEXT("Com_Model"));
					SocketDesc.pBoneName = "Bip01-Prop1";

					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_ExeWeapon", TEXT("Prototype_GameObject_ExeWeapon"), &SocketDesc))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
						return;
					}

					if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS, L"Layer_Effect", TEXT("Prototype_GameObject_ExeTrail"), &SocketDesc))) {
						MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Player, Player");
						return;
					}
				}
			}


		}
		else if (m_pPlayer->Get_SceneNum() == 6) {
			if (pGameInstance->Find_Layer_List(LEVEL_BOSS, L"Layer_Queen")->size() != 0) {
				CTransform* pQueenTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_BOSS, L"Layer_Queen", L"Com_Transform"));
				XMStoreFloat3(&m_CameraDesc.vEye, pQueenTrans->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.5f, 8.5f, -0.5f, 0.f));
				XMStoreFloat3(&m_CameraDesc.vAt, pQueenTrans->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 8.5f, 0.f, 0.f));
			}
			else {
				m_pPlayer->Set_PlayerMode(0);
				m_pPlayer->Set_Scene(7);
			}
			
		}
		else if (m_pPlayer->Get_SceneNum() == 7) {
			++Time;
			CTransform* pPlayerTrans = (CTransform*)(pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform"));

			XMStoreFloat3(&m_CameraDesc.vEye, pPlayerTrans->Get_State(CTransform::STATE_POSITION) + XMVectorSet(cosf(XMConvertToRadians(Time/2.f)), Time/300.f, sinf(XMConvertToRadians(Time/2.f)), 0.f));
			XMStoreFloat3(&m_CameraDesc.vAt, pPlayerTrans->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, Time / 300.f, 0.f, 0.f));

		}
	}


	_vector		vLook = XMLoadFloat3(&m_CameraDesc.vAt) - XMLoadFloat3(&m_CameraDesc.vEye);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMLoadFloat3(&m_CameraDesc.vAxisY), vLook);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(vLook, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	_vector		vPosition = XMVectorSetW(XMLoadFloat3(&m_CameraDesc.vEye), 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	
	__super::Tick(TimeDelta);

	Safe_Release(pTargetTransform);
	Safe_Release(pGameInstance);
}

void CCamera_Default::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CCamera_Default::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CCamera_Default * CCamera_Default::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CCamera_Default*	pInstance = new CCamera_Default(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Default::Clone(void * pArg)
{
	CCamera_Default*	pInstance = new CCamera_Default(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Default::Free()
{
	__super::Free();

}
