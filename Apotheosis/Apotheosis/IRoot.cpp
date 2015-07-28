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

#include "TyroIRoot.h"


	IRoot::IRoot()
		: m_pRenderer(DX10Renderer::getInstance())
		, m_pPhysicsSys(PhysicsSystem::getInstance())
		, m_pRootFactory(Factory::getInstancePtr())
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


		//Create the requisite DataPools in the factory
		m_pTextRenderPool = Factory::createDataPool<RenderTaskText>();
		m_pMeshComponentPool = Factory::createDataPool<MeshComponent>();
		m_pPhysicsComponentPool = Factory::createDataPool<PhysicsComponent>();
		m_pParticleComponentPool = Factory::createDataPool<ParticleComponent>();
		//Let all systems initialize their own access to the data pools in the factory that they require
		m_pRenderer->initFactory(); //Renderer creates all render tasks
		m_pPhysicsSys->initFactory();
	}

	bool IRoot::init(HWND _hWND, HINSTANCE _hInst)
	{

		if (!m_pRenderer->init(_hWND, &m_initializer))
			return false;

		m_pCamera = m_pRenderer->createCamera();


		//Functions to be implemented by derived object (application side initialization / creation)
		init();
		createScene();

		return true;
	}

	IRoot::~IRoot()
	{
	}


	//Release all memory
	void IRoot::shutDown()
	{

		m_pRenderer->shutDown();
		m_pPhysicsSys->shutDown();

		//Delete all data pools in the factory
		SAFE_DELETE(m_pTextRenderPool);
		SAFE_DELETE(m_pMeshComponentPool);
		SAFE_DELETE(m_pPhysicsComponentPool);
		SAFE_DELETE(m_pParticleComponentPool);

		//Delete the factory
		SAFE_DELETE(m_pRootFactory);
	}



	bool IRoot::executeOneFrame(bool _bGameIsPaused)
	{

		m_timer.tick();

		countFPS();


		if (!_bGameIsPaused)
			update((float)m_timer.getDeltaTime());	//This is pure virtual overridden by derived game root

		//Render the scene
		m_pRenderer->updateGameTime((float)m_timer.getGameTime());

		m_pRenderer->update((float)m_timer.getDeltaTime());

		m_pPhysicsSys->update((float)m_timer.getDeltaTime());

		return true;

	}


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


