// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2014 Media Design School 
// 
// File Name : D3DInputManager.h
// Description :  D3D Input Manager header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com


#pragma once

#define DIRECTINPUT_VERSION 0x0800

#ifndef __DI_MANAGER__
#define __DI_MANAGER__

#include "Util.h"
#include <dinput.h>


#define BUTTONDOWN(name, key) (name.rgbButtons[key] & 0x80) 


	//Interfaces with InputHandler 
	//Provides Keyboard / Mouse functionality
	class D3DInputManager
	{

		
		
	public: //Construction

		D3DInputManager();
		~D3DInputManager();


		
		bool init(HINSTANCE, HWND);


		//No copying
		D3DInputManager(const D3DInputManager&) = delete;
		D3DInputManager& operator= (const D3DInputManager&) = delete;

	private: //Data
		//For screen space coordinates
		HWND m_hWnd;

		IDirectInput*        m_pDI;		//Used to enumerate and create devices

		IDirectInputDevice8* m_pMouse;		//Mouse device
		DIMOUSESTATE2 m_mouseState;

		IDirectInputDevice* m_pKeyboard;	//Keyboard device
		char m_cKeyboardState[256];

	public: //Interface

		inline HWND getWindowHandle() { return m_hWnd; }

		//Poll devices
		void pollMouse();
		void pollKeyboard();

		//Check for specific mouse / keyboard buttons presses
		bool isMouseButtonDown(int);
		bool isKeyDown(char _cKey);

		//Mouse movement query
		inline float getCurMouseX() { return (float)m_mouseState.lX; }
		inline float getCurMouseY() { return (float)m_mouseState.lY; }

	};


#endif //__DI_MANAGER__