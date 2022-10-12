#include "..\Public\Navigation.h"

#include "Shader.h"
#include "Transform.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(CNavigation)

CNavigation::CNavigation()
{
}

HRESULT CNavigation::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar * pNavigationData, CTransform* pTransform)
{

	if (m_Cells.size() != 0) {
		for (auto& iter : m_Cells)
			Safe_Release(iter);
	}

	m_Cells.clear();

	Safe_Release(m_pTransform);

#ifdef _DEBUG
	Safe_Release(m_pDebugShader);
#endif // _DEBUG


	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(pNavigationData, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;
	
	while (true)
	{
		_float4		vPoint[CCell::POINT_END];

		ReadFile(hFile, vPoint, sizeof(_float4) * CCell::POINT_END, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell*			pCell = CCell::Create(pDevice, pDeviceContext, vPoint, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	m_pTransform = pTransform;
	Safe_AddRef(m_pTransform);

	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;

#ifdef _DEBUG	
	m_pDebugShader = CShader::Create(pDevice, pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXCOL_DECLARATION::Elements, VTXCOL_DECLARATION::iNumElements);
	if (nullptr == m_pDebugShader)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

_bool CNavigation::Move_OnNavigation(_fvector vPosition, _uint * pCurrentIndex, _bool bJump, _bool bPlayer, _bool bSmall)
{
	if (*pCurrentIndex >= m_Cells.size())
		return false;	

	_matrix		WorldMatrix = m_pTransform->Get_WorldMatrix();

	_int		iNeighborIndex = -1;

	if (true == m_Cells[*pCurrentIndex]->isIn(vPosition, WorldMatrix, &iNeighborIndex))
		return true;
	else
	{		
		/* 이웃이 있는지 없는지 검사를 하고. */
		/* 이웃이 있었다면. */
		if (-1 != iNeighborIndex)
		{
			while (true)
			{	
				if (-1 == iNeighborIndex)
					return false;

				if(true == m_Cells[iNeighborIndex]->isIn(vPosition, WorldMatrix, &iNeighborIndex))
				{
					if (m_Cells[iNeighborIndex]->Get_Option() == 1) {
						if (bJump) {
							*pCurrentIndex = iNeighborIndex;
							return true;
						}
						else
							return false;
					}
					else if (m_Cells[iNeighborIndex]->Get_Option() == 2) {
						if (bPlayer) {
							*pCurrentIndex = iNeighborIndex;
							return true;
						}
						else
							return false;
					}
					else if (m_Cells[iNeighborIndex]->Get_Option() == 3) {
						if (bSmall) {
							*pCurrentIndex = iNeighborIndex;
							return true;
						}
						else
							return false;
					}
					else if(m_Cells[iNeighborIndex]->Get_Option() != 1 && m_Cells[iNeighborIndex]->Get_Option() != 2 && m_Cells[iNeighborIndex]->Get_Option() != 3) {
						*pCurrentIndex = iNeighborIndex;
						return true;
					}
				}
			}
		}
		/* 이웃이 없다면. */
		else
			return false;

	}
	

	return _bool();
}

_float CNavigation::Compute_Height(_fvector vPosition, _uint * pCurrentIndex) {
	_vector		vPlane;

	vPlane = XMPlaneFromPoints(m_Cells[*pCurrentIndex]->Get_Point(CCell::POINT_A),
		m_Cells[*pCurrentIndex]->Get_Point(CCell::POINT_B),
		m_Cells[*pCurrentIndex]->Get_Point(CCell::POINT_C));


	return (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition) - XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	if (nullptr == m_pTransform)
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pDebugShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pDebugShader->Set_RawValue("g_ViewMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pDebugShader->Set_RawValue("g_ProjMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
	m_pDebugShader->Set_RawValue("g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4));

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Render(m_pDebugShader);
	}	

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}
HRESULT CNavigation::Render_Cell(_int iIndex)
{
	if (nullptr == m_pTransform)
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pDebugShader->Set_RawValue("g_WorldMatrix", &m_pTransform->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pDebugShader->Set_RawValue("g_ViewMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pDebugShader->Set_RawValue("g_ProjMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
	m_pDebugShader->Set_RawValue("g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4));

	m_Cells[iIndex]->Render(m_pDebugShader);

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;	
}

#endif // _DEBUG

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if(true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_AB, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_BC, pDestCell->Get_Index());
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_NeighborIndex(CCell::LINE_CA, pDestCell->Get_Index());
			}
		}
	}
	return S_OK;
}

void CNavigation::Free()
{
	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

	Safe_Release(m_pTransform);

#ifdef _DEBUG
	Safe_Release(m_pDebugShader);
#endif // _DEBUG
}
