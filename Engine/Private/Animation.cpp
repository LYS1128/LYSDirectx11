#include "..\Public\Animation.h"



CAnimation::CAnimation()
{

}

CAnimation::CAnimation(const CAnimation & rhs)
	: m_Channels(rhs.m_Channels)	
	, m_iNumChannels(rhs.m_iNumChannels)
	, m_Duration(rhs.m_Duration)
	, m_TickPerSecond(rhs.m_TickPerSecond)
	, m_TimeAcc(rhs.m_TimeAcc)
	, m_isFinished(rhs.m_isFinished)
{
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);

	strcpy_s(m_szName, rhs.m_szName);
	
}

HRESULT CAnimation::NativeConstruct_Prototype(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_Duration = pAIAnimation->mDuration;
	m_TickPerSecond = pAIAnimation->mTicksPerSecond;

	if (FAILED(Ready_Channels(pAIAnimation, Nodes)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimation::NativeConstruct(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	if (FAILED(Clone_Channels(pAIAnimation, Nodes)))
		return E_FAIL;

	return S_OK;
}


void CAnimation::Update(_double TimeDelta,_bool bCut, _float Speed)
{	
	/* 1. 현재 이 애니메이션이 재생되고 있는 시간을 계산한다. */
	/* 2. 이 시간대에 맞는 키프레임을 찾는다. */
	/* 3. 해당 키프레임사이의 뼈의 행렬값을 선형보간하여 구한다.  */
	/* 4. 이렇게 구한 행렬을 노드에 저장해둔다. */
	m_KeyFinished = false;
	m_PreTimeAcc = m_TimeAcc;
	m_TimeAcc += m_TickPerSecond * TimeDelta * Speed ;
	

	if (m_TimeAcc >= m_Duration)
	{
		m_KeyFinished = true;
		if (false == bCut) {
			m_isFinished = true;
			m_TimeAcc = 0.0;
		}
	}

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (true == m_isFinished && false == bCut)
			m_Channels[i]->Set_CurrentKeyFrame(0);

		m_Channels[i]->Compute_TransformationMatrix(m_TimeAcc);

		if (!strcmp(m_Channels[i]->Get_HierarchyNode()->Get_Name(), "Bip01-Head"))
			m_iCurrentKeyFrame = m_Channels[i]->Get_CurrentKeyFrameIndex();



	}
}

list<KEYFRAME> CAnimation::Pre_Change_Animation(_double TimeDelta)
{
	m_isFinished = false;

	m_TimeAcc += m_TickPerSecond * TimeDelta;


	if (m_TimeAcc >= m_Duration)
	{
		m_KeyFinished = true;
		m_isFinished = true;
		m_TimeAcc = 0.0;
	}

	list<KEYFRAME> PreKeylist;
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{

		KEYFRAME PreKey = m_Channels[i]->Get_LastKeyFrame(m_TimeAcc);
		PreKeylist.push_back(PreKey);
	}
	return PreKeylist;
}

_bool CAnimation::Change_Animation(list<KEYFRAME> PreKey,_double TimeDelta)
{
	m_TimeAcc += 10.0 * TimeDelta;

	if (m_TimeAcc >= 1)
	{
		m_isFinished = true;
	}

	auto iter = PreKey.begin();

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{		
		if (m_isFinished)
			break;

		m_Channels[i]->Change_Compute_TransformationMatrix(m_TimeAcc, *iter);
		++iter;
	}

	return m_isFinished;
}

HRESULT CAnimation::Ready_Channels(aiAnimation * pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*			pAIChannel = pAIAnimation->mChannels[i];

		CChannel*			pChannel = CChannel::Create(pAIChannel);
		if (nullptr == pChannel)
			return E_FAIL;

		auto	iter = find_if(Nodes.begin(), Nodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIChannel->mNodeName.data, pNode->Get_Name());
		});

		if (iter == Nodes.end())
			return E_FAIL;

		pChannel->SetUp_HierarchyNodePtr(*iter);		

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

HRESULT CAnimation::Clone_Channels(aiAnimation * pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	m_iNumChannels = pAIAnimation->mNumChannels;

	vector<CChannel*>		Channels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*			pAIChannel = pAIAnimation->mChannels[i];	

		CChannel*			pChannel = m_Channels[i]->Clone();
		if (nullptr == pChannel)
			return E_FAIL;

		auto	iter = find_if(Nodes.begin(), Nodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIChannel->mNodeName.data, pNode->Get_Name());
		});

		if (iter == Nodes.end())
			return E_FAIL;

		pChannel->SetUp_HierarchyNodePtr(*iter);

		Channels.push_back(pChannel);		

		Safe_Release(m_Channels[i]);
	}

	m_Channels.clear();

	m_Channels = Channels;

	return S_OK;
}

CAnimation * CAnimation::Create(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	CAnimation*	pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct_Prototype(pAIAnimation, Nodes)))
	{
		MSG_BOX(TEXT("Failed to Created CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation * CAnimation::Clone(aiAnimation * pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	CAnimation*	pInstance = new CAnimation(*this);

	if (FAILED(pInstance->NativeConstruct(pAIAnimation, Nodes)))
	{
		MSG_BOX(TEXT("Failed to Created CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}

