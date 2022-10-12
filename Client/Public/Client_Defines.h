#pragma once

namespace Client
{
	const unsigned int	g_iWinCX = 1280;
	const unsigned int	g_iWinCY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_BOSS, LEVEL_END };
	enum MODE { PMODE_DEFAULT, PMODE_SUPER, PMODE_SMALL, PMODE_END };
}

#include <process.h>

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

extern unsigned int g_iLevel;
extern unsigned int g_iMode;

using namespace Client;


