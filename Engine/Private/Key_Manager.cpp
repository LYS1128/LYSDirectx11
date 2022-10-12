//#include "stdafx.h"
#include "../Public/Key_Manager.h"

IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager() {
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

_bool CKey_Manager::Key_Pressing(_uint iKey) {
	if (GetAsyncKeyState(iKey) & 0x8000) {
		return true;
	}
	return false;
}

_bool CKey_Manager::Key_Down(_uint iKey) {
	if (!m_bKeyState[iKey] && (GetAsyncKeyState(iKey) & 0x8000)) {
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}
	if (m_bKeyState[iKey] && !(GetAsyncKeyState(iKey) & 0x8000)) {
		m_bKeyState[iKey] = !m_bKeyState[iKey];
	}
		return false;
}

_bool CKey_Manager::Key_Up(_uint iKey) {
	if (m_bKeyState[iKey] && !(GetAsyncKeyState(iKey) & 0x8000)) {
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}
	if (!m_bKeyState[iKey] && (GetAsyncKeyState(iKey) & 0x8000)) {
		m_bKeyState[iKey] = !m_bKeyState[iKey];
	}
	return false;
}

void CKey_Manager::Free()
{
}