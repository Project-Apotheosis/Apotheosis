// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : IRoot.h
// Description :  IRoot header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#pragma once

#ifndef __IRoot_H__
#define __IRoot_H__

#include <windows.h>
#include "Timer.h"
#include "DX10Renderer.h"
#include "PhysicsSystem.h"
#include "Util.h"
#include "InputHandler.h"


	class AppWin32;
	//Object to be used in winmain to start and loop the game
	class IRoot
	{

	public: //Construction

		IRoot();
		virtual ~IRoot();
		bool init(HWND, HINSTANCE);
		//Called before createScene(), after Renderer and InputManager initialized
		virtual bool init() = 0;
		virtual void shutDown();

	private: //Friend

		friend class AppWin32;


	protected: //Data

		Camera* m_pCamera{};

		string m_sFrameStats;
		Timer m_timer;
		DX10Renderer*		m_pRenderer{};
		PhysicsSystem*		m_pPhysicsSystem{};
		InputHandler*		m_pInputHandler{};
		D3DInitializer		m_initializer; //Gives Win32App and Renderer details about screen.


	public: //Interface

		bool executeOneFrame(bool _bGameIsPaused);

	protected: //Internals

		//Populate SceneManager with objects, renderables and entities here
		virtual void createScene() = 0;
		//Add game logic in this call
		virtual void update(float _fDeltaTime) = 0;


	private:

		//Moniter FPS and store in wstring member 
		void countFPS();
	};

#endif //__IRoot_H__