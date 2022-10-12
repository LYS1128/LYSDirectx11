#include "..\Public\Transform.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

_float3 CTransform::Get_Scale()
{
	_float3		vScale;

	vScale.x = XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT)));
	vScale.y = XMVectorGetX(XMVector3Length(Get_State(STATE_UP)));
	vScale.z = XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK)));

	return vScale;
}

_matrix CTransform::Get_WorldMatrix()
{
	return XMLoadFloat4x4(&m_WorldMatrix);
}

_matrix CTransform::Get_WorldMatrixInverse()
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
}

_float4x4 CTransform::Get_WorldFloat4x4()
{
	return m_WorldMatrix;
}

_float4x4 CTransform::Get_WorldFloat4x4_TP()
{
	_float4x4	WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	return WorldMatrix;
}

HRESULT CTransform::NativeConstruct_Prototype(void* pArg)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

HRESULT CTransform::Bind_WorldMatrixOnShader(CShader * pShader, const char * pConstantName)
{
	if (nullptr == pShader)
		return E_FAIL;

	_float4x4		WorldMatrixTP;

	XMStoreFloat4x4(&WorldMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	pShader->Set_RawValue(pConstantName, &WorldMatrixTP, sizeof(_float4x4));
	
	return S_OK;
}

HRESULT CTransform::Navigationheight(_uint * pCurrentIndex) {
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	CNavigation*		pNavigation = GET_INSTANCE(CNavigation);
	_float		fHeight = pNavigation->Compute_Height(vPosition, pCurrentIndex);
	vPosition = XMVectorSetY(vPosition, fHeight);
	Set_State(CTransform::STATE_POSITION, vPosition);
	RELEASE_INSTANCE(CNavigation);
	return S_OK;
}

_float CTransform::Compute_Height(_uint * pCurrentIndex) {
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	CNavigation*		pNavigation = GET_INSTANCE(CNavigation);
	_float		fHeight = pNavigation->Compute_Height(vPosition, pCurrentIndex);

	RELEASE_INSTANCE(CNavigation);
	return fHeight;
}

_uint CTransform::NaviOption(_uint * pCurrentIndex) {
	CNavigation*		pNavigation = GET_INSTANCE(CNavigation);
	_uint		iOption = pNavigation->NaviOption(pCurrentIndex);

	RELEASE_INSTANCE(CNavigation);
	return iOption;
}

HRESULT CTransform::Go_Straight(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;


	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_StraightOnNavigation(_double TimeDelta, _uint * pCurrentIndex, _bool bJump, _bool bPlayer, _bool bSmall )
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	CNavigation*		pNavigation = GET_INSTANCE(CNavigation);

	if (true == pNavigation->Move_OnNavigation(vPosition, pCurrentIndex, bJump,bPlayer, bSmall)) {
		Set_State(CTransform::STATE_POSITION, vPosition);
	}

	RELEASE_INSTANCE(CNavigation)

		return S_OK;
}

HRESULT CTransform::Go_OnNavigation(_double TimeDelta, _vector vGo, _uint * pCurrentIndex, _bool bJump, _bool bPlayer, _bool bSmall)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vGo) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	CNavigation*		pNavigation = GET_INSTANCE(CNavigation);

	if (true == pNavigation->Move_OnNavigation(vPosition, pCurrentIndex, bJump, bPlayer, bSmall)) {
		Set_State(CTransform::STATE_POSITION, vPosition);
	}

	RELEASE_INSTANCE(CNavigation)

		return S_OK;
}

HRESULT CTransform::Go_Left(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_LeftOnNavigation(_double TimeDelta, _uint * pCurrentIndex, _bool bJump, _bool bPlayer, _bool bSmall)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	CNavigation*		pNavigation = GET_INSTANCE(CNavigation);

	if (true == pNavigation->Move_OnNavigation(vPosition, pCurrentIndex,bJump,bPlayer, bSmall)) {
		_float		fHeight = pNavigation->Compute_Height(vPosition, pCurrentIndex);
		vPosition = XMVectorSetY(vPosition, fHeight);
		Set_State(CTransform::STATE_POSITION, vPosition);

	}

	RELEASE_INSTANCE(CNavigation)

		return S_OK;
}

HRESULT CTransform::Go_Right(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_RightOnNavigation(_double TimeDelta, _uint * pCurrentIndex, _bool bJump, _bool bPlayer, _bool bSmall)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	CNavigation*		pNavigation = GET_INSTANCE(CNavigation);

	if (true == pNavigation->Move_OnNavigation(vPosition, pCurrentIndex, bJump,bPlayer, bSmall)) {
		_float		fHeight = pNavigation->Compute_Height(vPosition, pCurrentIndex);
		vPosition = XMVectorSetY(vPosition, fHeight);
		Set_State(CTransform::STATE_POSITION, vPosition);

	}

	RELEASE_INSTANCE(CNavigation)

		return S_OK;
}

HRESULT CTransform::Go_BackWard(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_BackWardOnNavigation(_double TimeDelta, _uint * pCurrentIndex, _bool bJump, _bool bPlayer, _bool bSmall)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	CNavigation*		pNavigation = GET_INSTANCE(CNavigation);

	if (true == pNavigation->Move_OnNavigation(vPosition, pCurrentIndex, bJump,bPlayer, bSmall)) {
		_float		fHeight = pNavigation->Compute_Height(vPosition, pCurrentIndex);
		vPosition = XMVectorSetY(vPosition, fHeight);
		Set_State(CTransform::STATE_POSITION, vPosition);

	}

	RELEASE_INSTANCE(CNavigation)

		return S_OK;
}

HRESULT CTransform::Go_Down(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vUp = Get_State(CTransform::STATE_UP);

	vPosition -= XMVector3Normalize(vUp) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Scaled(const _float3 & vScale)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3Normalize(vRight) * vScale.x;
	vUp = XMVector3Normalize(vUp) * vScale.y;
	vLook = XMVector3Normalize(vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	_matrix		RotationMatrix;
	RotationMatrix = XMMatrixRotationAxis(vAxis, _float(m_TransformDesc.RotationPerSec * TimeDelta));

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotation(_fvector vAxis, _double Radian)
{
	_float3		vScale = Get_Scale();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix;
	RotationMatrix = XMMatrixRotationAxis(vAxis, _float(Radian));

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Collide(_float3 MySize, _float3 ObjSize, _vector ObjPos)
{
	_vector MyPos = Get_State(CTransform::STATE_POSITION);
	_float XDist = XMVectorGetX(MyPos) - XMVectorGetX(ObjPos);
	_float YDist = XMVectorGetY(MyPos) - XMVectorGetY(ObjPos);
	_float ZDist = XMVectorGetZ(MyPos) - XMVectorGetZ(ObjPos);

	
		if (MySize.x*0.5 + ObjSize.x*0.5 - fabs(XDist) <= MySize.z*0.5 + ObjSize.z*0.5 - fabs(ZDist)) {//X방향충돌
			if (XDist < 0) {
				_float MyPosX = XMVectorGetX(MyPos) - (MySize.x*0.5 + ObjSize.x*0.5 + XDist);
				MyPos = XMVectorSetX(MyPos, MyPosX);
				Set_State(CTransform::STATE_POSITION, MyPos);
			}
			else {
				_float MyPosX = XMVectorGetX(MyPos) + (MySize.x*0.5 + ObjSize.x*0.5 - XDist);
				MyPos = XMVectorSetX(MyPos, MyPosX);
				Set_State(CTransform::STATE_POSITION, MyPos);
			}
		}
		else {
			if (ZDist < 0) {
				_float MyPosZ = XMVectorGetZ(MyPos) - (MySize.z*0.5 + ObjSize.z*0.5 + ZDist);
				MyPos = XMVectorSetZ(MyPos, MyPosZ);
				Set_State(CTransform::STATE_POSITION, MyPos);
			}
			else {
				_float MyPosZ = XMVectorGetZ(MyPos) + (MySize.z*0.5 + ObjSize.z*0.5 - ZDist);
				MyPos = XMVectorSetZ(MyPos, MyPosZ);
				Set_State(CTransform::STATE_POSITION, MyPos);
			}
		}


	return S_OK;
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg)
{
	CTransform*	pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	return nullptr;
}

void CTransform::Free()
{
	__super::Free();

}

