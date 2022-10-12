#pragma once
#include "fmod/fmod.h"
#include "Base.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_HALF 0.5f
#define SOUND_THREE 0.3f
#define SOUND_DEFAULT 0.2f
#define SOUND_WEIGHT 0.1f
#define SOUND_SMALL 0.05f

BEGIN(Engine)

class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

public:
	enum CHANNELID { BGM, BIRD, WATER, PLAYER_FOOTSTEPL, PLAYER_FOOTSTEPR, PLAYER_WEAPON, PLAYER_WEAPON_SUB, PLAYER_WEAPON_HIT,PLAYER_JUMP, PLAYER_EFFECT, PLAYER_BREATH, 
		SHELL_EFFECT, TOOTH_EFFECT, MONSTER_EFFECT, SCENE_SOUND1, SCENE_SOUND2, MAXCHANNEL };

private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Initialize();

public:
	HRESULT  VolumeUp(_uint eID, _float _vol);
	HRESULT  VolumeDown(_uint eID, _float _vol);
	HRESULT  BGMVolumeUp(_float _vol);
	HRESULT  BGMVolumeDown(_float _vol);
	HRESULT  Pause(_uint eID);
	HRESULT SoundPlay(TCHAR* pSoundKey, _uint eID, _float _vol);
	HRESULT PlayBGM(TCHAR* pSoundKey);
	HRESULT StopSound(_uint eID);
	HRESULT StopAll();

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

private:
	HRESULT LoadSoundFile();

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem = nullptr;

	_bool		m_bPause = false;

public:
	virtual void Free() override;
};

END

