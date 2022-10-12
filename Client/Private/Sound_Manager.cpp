#include "stdafx.h"
#include "../Public/Sound_Manager.h"

IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager() {
	m_pSystem = nullptr;
}

HRESULT CSound_Manager::NativeConsruct() {
	FMOD::System* pSystem = nullptr;
	FMOD::System_Create(&pSystem);

	m_pSystem = (FMOD_SYSTEM*)pSystem;
	if (nullptr == m_pSystem) {
		MSG_BOX(L"Failed To CSound_Manager : NativeConsruct");
		return E_FAIL;
	}
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
	return S_OK;
}

int CSound_Manager::VolumeUp(CHANNELID eID, _float _vol) {
	if (m_volume < SOUND_MAX) {
		m_volume += _vol;
	}
	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);
	return 0;
}

int CSound_Manager::VolumeDown(CHANNELID eID, _float _vol) {
	if (m_volume > SOUND_MIN) {
		m_volume -= _vol;
	}
	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);
	return 0;
}

int CSound_Manager::BGMVolumeUp(_float _vol) {
	if (m_BGMvolume < SOUND_MAX) {
		m_BGMvolume += _vol;
	}
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);
	return 0;
}

int CSound_Manager::BGMVolumeDown(_float _vol) {
	if (m_BGMvolume > SOUND_MIN) {
		m_BGMvolume -= _vol;
	}
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);
	return 0;
}

int CSound_Manager::Pause(CHANNELID eID) {
	m_bPause = !m_bPause;
	FMOD_Channel_SetPaused(m_pChannelArr[eID], m_bPause);
	return 0;
}

void CSound_Manager::PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float _vol) {
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter) {
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end()) {
		return;
	}
	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr,FALSE, &m_pChannelArr[eID]);
		if (_vol >= SOUND_MAX) {
			_vol = 1.f;
		}
		else if (_vol <= SOUND_MIN) {
			_vol = 0.f;
		}
		FMOD_Channel_SetVolume(m_pChannelArr[eID], _vol);
	}
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(TCHAR* pSoundKey) {
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter) {
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end()) {
		return;
	}
	FMOD_System_PlaySound(m_pSystem,iter->second,nullptr, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], 0.1f);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::StopSound(CHANNELID eID) {
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::StopAll() {
	for (int i = 0; i < MAXCHANNEL; ++i) {
		FMOD_Channel_Stop(m_pChannelArr[i]);
	}
}

void CSound_Manager::LoadSoundFile() {
	_tfinddata64_t fd;
	__int64 handle = _tfindfirst64(L"../../Sound/*.wav", &fd);
	if (handle == -1 || handle == 0) {
		return;
	}
	int iResult = 0;

	char szCurPath[128] = "../../Sound/";
	char szFullPath[128] = "";
	char szFilename[MAX_PATH];
	while (iResult != -1) {
		WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, szFilename);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
		if (eRes == FMOD_OK) {
			int iLength = _uint(strlen(szFilename) + 1);

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, szFilename, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _tfindnext64(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}

void CSound_Manager::Free() {
	for (auto& Mypair : m_mapSound)	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}