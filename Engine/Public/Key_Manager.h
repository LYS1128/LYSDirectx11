#pragma once
#include "Base.h"
#include "Engine_Defines.h"
#include "Input_Device.h"

BEGIN(Engine)

class CKey_Manager final : public CBase {
	DECLARE_SINGLETON(CKey_Manager)
private:
	CKey_Manager();
	virtual ~CKey_Manager() =default;
public:
	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);
private:
	_bool m_bKeyState[VK_MAX];
public:
	virtual void Free() override;
};

END