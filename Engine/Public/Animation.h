#pragma once

#include "Base.h"
#include "Channel.h"
#include "HierarchyNode.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT NativeConstruct_Prototype(aiAnimation* pAIAnimation, vector<class CHierarchyNode*> Nodes);
	HRESULT NativeConstruct(aiAnimation* pAIAnimation, vector<class CHierarchyNode*> Nodes);
	void Update(_double TimeDelta,_bool bCut, _float Speed);
public:
	list<KEYFRAME> Pre_Change_Animation(_double TimeDelta);
	_bool Change_Animation(list<KEYFRAME> PreMat,_double TimeDelta);

	_uint Get_CurrnetKeyFrame() { return m_iCurrentKeyFrame; }
	_bool Get_KeyFrameENd() { return m_KeyFinished; }
	void Set_KeyFrameEnd(_bool End) { m_KeyFinished = End; }
	void Set_KeyFrameZero() {
		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			m_Channels[i]->Set_CurrentKeyFrame(0);
		}
	}

private:
	char					m_szName[MAX_PATH] = "";
	_double					m_Duration = 0.0; /* 애니메이션 하나를 재생하는데 걸리는 총 시간. */
	_double					m_TickPerSecond = 0.0;

	_double					m_TimeAcc = 0.0;
	_bool					m_isFinished = false;
	_bool					m_KeyFinished = false;
	_uint m_iCurrentKeyFrame = 0;

private:
	_uint								m_iNumChannels = 0;
	_double							m_PreTimeAcc = 0;

	vector<class CChannel*>				m_Channels;
	typedef vector<class CChannel*>		CHANNELS;

private:
	HRESULT Ready_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT Clone_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);

public:
	void Set_TimeAcc_Zero() {
		m_TimeAcc = 0;
	}


public:
	static CAnimation* Create(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	CAnimation* Clone(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	virtual void Free() override;
};

END