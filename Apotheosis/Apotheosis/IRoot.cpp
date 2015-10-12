// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name :	IRoot.cpp
// Description :  IRoot cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "IRoot.h"
#include <sstream>

	/**
	*	Constructor
	*	@author Serge Radinovich
	*/
	IRoot::IRoot()
		: m_pRenderer(DX10Renderer::getInstance())
		, m_pPhysicsSystem(PhysicsSystem::getInstance())
		, m_pInputHandler(InputHandler::getInstance())
		, m_pAudioManager(AudioManager::getInstance())
	{
		//Timer
		m_timer.reset();
		//m_timer.start();

		//Initializer
		ZeroMemory(&m_initializer, sizeof(m_initializer));
		//These details can be edited by derived type of IRoot constructor so that Win32App gets game specific details
		m_initializer.iWidth = SCREEN_WIDTH_WINDOWED;
		m_initializer.iHeight = SCREEN_HEIGHT_WINDOWED;
		m_initializer.bIsWindowed = true;

	}

	/**
	*	Initialize the root
	*	@author Serge Radinovich
	*	@param  _hWnd		Handle to window the root is relating to
	*	@param  _hInst		Handle to instance the root is relating to
	*/
	bool IRoot::init(HWND _hWND, HINSTANCE _hInst)
	{

		if (!m_pRenderer->init(_hWND, &m_initializer))
			return false;

		m_pInputHandler->init(_hInst, _hWND);

		m_pCamera = m_pRenderer->createCamera();

		if (!m_pAudioManager->init())
			return false;


		//Functions to be implemented by derived object (application side initialization / creation)
		init();
		createScene();

		return true;
	}

	/**
	*	Destructor
	*	@author Serge Radinovich
	*/
	IRoot::~IRoot()
	{
	}


	/**
	*	Shutdown singleton
	*	@author Serge Radinovich
	*/
	void IRoot::shutDown()
	{
		m_pInputHandler->shutDown();
		m_pRenderer->shutDown();
		m_pPhysicsSystem->shutDown();
		m_pAudioManager->shutDown();
	}


	/**
	*	Run a frame
	*	@author Serge Radinovich
	*	@param  _bGameIsPaused		Whether game is paused or not
	*/
	bool IRoot::executeOneFrame(bool _bGameIsPaused)
	{

		m_timer.tick();

		countFPS();


		if (!_bGameIsPaused)
			update((float)m_timer.getDeltaTime());	//This is pure virtual overridden by derived game root

		//Render the scene

		m_pRenderer->update((float)m_timer.getDeltaTime());
		m_pPhysicsSystem->update((float)m_timer.getDeltaTime());
		m_pAudioManager->Update();

		return true;

	}

	/**
	*	For fps counter
	*	@author Serge Radinovich
	*/
	void IRoot::countFPS()
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.

		static int s_iFrameCount = 0;
		static float s_fBase = 0.0f;
		s_iFrameCount++;

		// Compute averages over one second period.
		if ((m_timer.getGameTime() - s_fBase) >= 1.0f)
		{
			float _fFPS = (float)s_iFrameCount; // fps = frameCnt / 1
			float _fMillSecPerFrame = 1000.0f / _fFPS;

			std::ostringstream outs;
			outs.precision(6);
			outs << "FPS: " << _fFPS << "\n"
				<< "Milliseconds: Per Frame: " << _fMillSecPerFrame;
			m_sFrameStats = outs.str();

			// Reset for next average.
			s_iFrameCount = 0;
			s_fBase += 1.0f;
		}
	}


