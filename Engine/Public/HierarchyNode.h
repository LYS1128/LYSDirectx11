#pragma once

#include "Base.h"

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	const char* Get_Name() {
		return m_szName;
	}

	_uint Get_Depth() const {
		return m_iDepth;
	}

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombinedMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	_float4x4* Get_CombinedFloat4x4Ptr() {
		return &m_CombinedTransformationMatrix;
	}

public:
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

public:
	HRESULT NativeConstruct(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);
	void Update_CombinedTransformationMatrix();

private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iDepth = 0;
	CHierarchyNode*	m_pParent = nullptr;	
	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformationMatrix; /* 부모기준 나의 상태. */
	_float4x4		m_CombinedTransformationMatrix; /* 나의 상태  * 부모상태 */

public:
	static CHierarchyNode* Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);
	virtual void Free() override;
};

END