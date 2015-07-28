// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : AppWin32.h
// Description :  AppWin32 header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#pragma once

#ifndef __APP_WIN32__
#define __APP_WIN32__

#include <Windows.h>
#include <stdlib.h>//srand()
#include <time.h>  //srand()
#include "TyroIRoot.h"

#include "TyroUtil.h"

#include <tchar.h>
#include <time.h>

namespace Tyro
{
	//Win32 Application
	//Every Project will get a AppWin32* and run it with a derived IRoot
	class AppWin32
	{

	private: //Construction (singleton)

		AppWin32();
		static AppWin32* s_pInstance;

	public:
		//Get the instance for main()
		static AppWin32* getInstancePtr()
		{
			if (s_pInstance == nullptr)
			{
				s_pInstance = new AppWin32();
			}
			return s_pInstance;
		}

		//Give the app a derived IRoot through [in] base pointer
		bool init(HINSTANCE _hInst, IRoot* _pIRoot);

		void shutDown();
		~AppWin32();

		//Disallow copying
		AppWin32(const AppWin32& _rBB) = delete;
		AppWin32& operator=(const AppWin32& _rBB) = delete;

	private: //Data

		HINSTANCE m_hInst;
		HWND m_hWnd;
		MSG m_msg;

		IRoot* m_pRoot{};

		bool m_bPaused{ false }; //When application is deactivated through message pump
		bool m_bResizing{ false };
		bool m_bMinimized{ false };
		bool m_bMaximized{ false };

	public: //Interface

		//Message pump
		void run();

	private: //Internals

		//Friend CALLBACK function which is the way the application accesses the member function message pump
		friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
		//Member function message pump accessed from friend function CALLBACK
		LRESULT CALLBACK mWindowProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);

		bool    initWindow(HINSTANCE hInstance, const D3DInitializer*);
	};
}
#endif //__APP_WIN32__
