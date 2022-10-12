#include "..\Public\Cell.h"
#include "VIBuffer_Line.h"
#include "Shader.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CCell::NativeConstruct(const _float4 * pPoints, _int iIndex)
{
	m_iIndex = iIndex;

	memcpy(m_vPoint4, pPoints, sizeof(_float4) * POINT_END);
	m_iOption = m_vPoint4[0].w;

	m_vPoint[0] = _float3(m_vPoint4[0].x, m_vPoint4[0].y, m_vPoint4[0].z);
	m_vPoint[1] = _float3(m_vPoint4[1].x, m_vPoint4[1].y, m_vPoint4[1].z);
	m_vPoint[2] = _float3(m_vPoint4[2].x, m_vPoint4[2].y, m_vPoint4[2].z);

#ifdef _DEBUG
	_float3		vPoints[4] = {
		m_vPoint[0], 
		m_vPoint[1],
		m_vPoint[2],
		m_vPoint[0]
	};

	m_pDebugBuffer = CVIBuffer_Line::Create(m_pDevice, m_pDeviceContext, vPoints, 4, &_float4(1.f, 1.f, 1.f, 1.f));
	if (nullptr == m_pDebugBuffer)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}



_bool CCell::isIn(_fvector vPosition, _fmatrix WorldMatrix, _int* pNeighborIndex)
{
	_vector		vPointInWorld[POINT_END + 1];

	for (_uint i = 0; i < POINT_END; ++i)
		vPointInWorld[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), WorldMatrix);
	vPointInWorld[POINT_END] = vPointInWorld[0];

	_vector		vLine[LINE_END];

	for (_uint i = 0; i < LINE_END; ++i)
	{
		vLine[i] = vPointInWorld[i + 1] - vPointInWorld[i];

		_vector vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine[i]) * -1.f, 0.f, XMVectorGetX(vLine[i]), 0.f));
		_vector vDir = XMVector3Normalize(vPosition - vPointInWorld[i]);

		if (0 < XMVectorGetX(XMVector3Dot(vNormal, vDir)))
		{
			*pNeighborIndex = m_iNeighbor[i];			

			return false;
		}
	}

	return true;
}

_bool CCell::Compare_Points(_vector vSour, _vector vDest)
{
	if (true == XMVector3Equal(vSour, XMLoadFloat3(&m_vPoint[POINT_A])))
	{
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoint[POINT_B])))
			return true;
		
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoint[POINT_C])))
			return true;		
	}

	if (true == XMVector3Equal(vSour, XMLoadFloat3(&m_vPoint[POINT_B])))
	{
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoint[POINT_A])))
			return true;

		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoint[POINT_C])))
			return true;
	}

	if (true == XMVector3Equal(vSour, XMLoadFloat3(&m_vPoint[POINT_C])))
	{
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoint[POINT_A])))
			return true;

		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoint[POINT_B])))
			return true;
	}

	return false;	
}

#ifdef _DEBUG

HRESULT CCell::Render(CShader * pShader)
{
	if (nullptr == m_pDebugBuffer)
		return E_FAIL;		

	pShader->Begin(0);

	m_pDebugBuffer->Render();
	
	return S_OK;
}

#endif // _DEBUG
CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _float4 * pPoints, _int iIndex)
{
	CCell*	pInstance = new CCell(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pPoints, iIndex)))
	{
		MSG_BOX(TEXT("Failed to Created CSky"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	
#ifdef _DEBUG
	Safe_Release(m_pDebugBuffer);
#endif // _DEBUG

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	
}
