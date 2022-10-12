#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "PIpeLine.h"
#include "Font_Manager.h"
#include "Light_Manager.h"
#include "Key_Manager.h"
#include "Navigation.h"
#include "Frustum.h"
#include "Target_Manager.h"
#include "Sound_Manager.h"

/* 클라이언트에 보여줘야할 인터페이스를 보관하고. 보여준다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance : public CBase {

	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, CGraphic_Device::GRAPHICDESC GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	void Tick_Engine(_double TimeDelta);
	HRESULT Render_Engine();
	HRESULT Render_Begin(_float4 vClearColor);
	HRESULT Render_End();
	HRESULT Clear_LevelResource(_uint iLevelIndex);

public: /* For.Graphic_Device */


public: /* For.Input_Device */
	_char Get_DIKeyState(_uchar byKeyID);
	_char Get_DIMButtonState(CInput_Device::MOUSEBUTTONSTATE eDIMBState);
	_long Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eDIMMState);

	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);

public: /* For.Timer_Manager */
	HRESULT Add_Timers(const _tchar* pTimerTag);
	_double Compute_TimeDelta(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pLevel);

	public: /*For.Font_Manager */ 
	HRESULT Add_Fonts(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Fonts(const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _fvector vColor, float rotation, _float2 const & Float2Zero, float scale);

public: /* For.Object_Manager */
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	list<CGameObject*>* Find_Layer_List(_uint iLevelIndex, const _tchar * pLayerTag);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);

public: /* For.PipeLine */
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState);
	_vector Get_CamPosition();
	_float4 Get_CamPositionFloat4();

public: /*For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	HRESULT Add_Lights(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);

public: /* For.Navigation */
	HRESULT Initialize_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNavigationData, CTransform* pTransform);
	HRESULT Render_Navigation();
	HRESULT Render_NavigationCell(_uint iIndex);
	HRESULT Set_CellOption(_uint pCurrentIndex, _uint Option);

public: /*For.Frustum */
	_bool isInFrustum_WorldSpace(_vector vWorldPos, _float fRange = 0.f);

public: /* for.Target_Manager */
	ID3D11ShaderResourceView* Get_RenderTargetSRV(const _tchar* pTargetTag) const;

public: /*For.Sound_Manager*/
	HRESULT Add_Sounds();
	HRESULT  VolumeUp(_uint iSoundIndex, _float _vol);
	HRESULT  VolumeDown(_uint iSoundIndex, _float _vol);
	HRESULT  BGMVolumeUp(_float _vol);
	HRESULT  BGMVolumeDown(_float _vol);
	HRESULT  Pause(_uint iSoundIndex);
	HRESULT SoundPlay(TCHAR* pSoundKey, _uint iSoundIndex, _float _vol);
	HRESULT PlayBGM(TCHAR* pSoundKey);
	HRESULT StopSound(_uint iSoundIndex);
	HRESULT StopAll();

private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CFont_Manager*				m_pFont_Manager = nullptr;
	CPipeLine*					m_pPipeLine = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
	CKey_Manager*				m_pKey_Manager = nullptr;
	CNavigation*				m_pNavigation = nullptr;
	CFrustum*					m_pFrustum = nullptr;
	CTarget_Manager*			m_pTarget_Manager = nullptr;
	CSound_Manager*				m_pSound_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END