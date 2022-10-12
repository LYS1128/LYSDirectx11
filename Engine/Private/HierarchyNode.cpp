#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

HRESULT CHierarchyNode::NativeConstruct(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	//3@모델로딩중에 이쪽으로 들어올 수 있다
	//pAINode는 노드 정보이고, pParent(부모)와 iDepth(깊이)는 받아오지 않았을 경우 각각 nullptr과 0이다

	//받아온 정보들을 멤버변수에 저장한다
	strcpy_s(m_szName, pAINode->mName.data);

	m_pParent = pParent;

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));	

	//부모 기준 나의 상태를 필요한 형식으로 바꿔서 저장한다 머 전치하고 형식바꾸고난리낫음
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	m_iDepth = iDepth;

	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix()
{
	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix));
}

CHierarchyNode * CHierarchyNode::Create(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode*	pInstance = new CHierarchyNode();

	if (FAILED(pInstance->NativeConstruct(pAINode, pParent, iDepth)))
	{
		MSG_BOX(TEXT("Failed to Created CHierarchyNode"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Free()
{

}
