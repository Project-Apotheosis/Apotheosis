// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : winmain.cpp
// Description :  winmain cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#ifdef _DEBUG
#include <vld.h>
#endif

#include "AppWin32.h"
#include "GameRoot.h"


int WINAPI _tWinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PTSTR _lpCmdLine, int _iCmdShow)
{
	AppWin32* _app = AppWin32::getInstancePtr();

	GameRoot _root;

	if (_app->init(_hInstance, &_root))
		_app->run();

	_app->shutDown();
}