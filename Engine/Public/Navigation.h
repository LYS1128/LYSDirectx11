#pragma once

#include "Base.h"
#include "Cell.h"

BEGIN(Engine)

class CNavigation final : public CBase
{
	DECLARE_SINGLETON(CNavigation)
private:
	CNavigation();
	virtual ~CNavigation() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNavigationData, class CTransform* pTranform);
	_bool Move_OnNavigation(_fvector vPosition, _uint* pCurrentIndex, _bool bJump, _bool bPlayer, _bool bSmall);
	_float Compute_Height(_fvector vPosition, _uint * pCurrentIndex);

#ifdef _DEBUG
public:
	HRESULT Render();
	HRESULT Render_Cell(_int iIndex);

public:
	_uint NaviOption(_uint * pCurrentIndex) {
		return m_Cells[*pCurrentIndex]->Get_Option();
	}
	void Set_CellOption(_uint * pCurrentIndex, _uint Option) {
		m_Cells[*pCurrentIndex]->Set_Option(Option);
	}

#endif // _DEBUG

private:
	vector<class CCell*>				m_Cells;
	typedef vector<class CCell*>		CELLS;


	class CTransform*			m_pTransform = nullptr;

#ifdef _DEBUG
private:
	class CShader*				m_pDebugShader = nullptr;
#endif // _DEBUG

private:
	HRESULT SetUp_Neighbor();


public:
	virtual void Free() override;
};

END