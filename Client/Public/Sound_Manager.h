#pragma once
#include "Base.h"
#include "Client_Defines.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

BEGIN(Client)

class CSound_Manager final : public CBase {
DECLARE_SINGLETON(CSound_Manager)
public:
	enum CHANNELID { BGM, PLAYER, EFFECT,UI,SYSTEM_EFFECT, SYSTEM_EFFECT2, SYSTEM_EFFECT3, MAXCHANNEL };
private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT NativeConsruct();

public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

private:
	void LoadSoundFile();

private:

	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	FMOD_SYSTEM* m_pSystem;
	_bool	m_bPause = false;
public:
	virtual void Free() override;
};

END