#pragma once

#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCell() = default;

public:
	_int Get_Index() const {
		return m_iIndex;
	}

	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoint[ePoint]);
	}

	_uint Get_Option() {
		return m_iOption;
	}

	void Set_NeighborIndex(LINE eLine, _int iIndex) {
		m_iNeighbor[eLine] = iIndex;
	}

	void Set_Option(_uint Option) {
		m_iOption = Option;
	}

public:
	HRESULT NativeConstruct(const _float4* pPoints, _int iIndex);
	_bool isIn(_fvector vPosition, _fmatrix WorldMatrix, _int* pNeighborIndex);

	_bool Compare_Points(_vector vSour, _vector vDest);

#ifdef _DEBUG
public:
	HRESULT Render(class CShader* pShader);
#endif // _DEBUG

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

private:
	_float4				m_vPoint4[POINT_END];
	_float3				m_vPoint[POINT_END];
	_int				m_iIndex = 0;
	_int				m_iOption = 0;

private:
	_int				m_iNeighbor[LINE_END] = { -1, -1, -1 };

#ifdef _DEBUG
private:
	class CVIBuffer_Line*		m_pDebugBuffer = nullptr;
#endif // _DEBUG

public:
	static CCell* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _float4* pPoints, _int iIndex);
	virtual void Free() override;
};

END