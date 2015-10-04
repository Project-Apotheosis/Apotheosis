// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : GameRoot.cpp
// Description :  GameRoot cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "GameRoot.h"

#include "Util.h"


/**
*	Constructor
*	Set up client window size here
*	@author Serge Radinovich
*/
GameRoot::GameRoot()
{
	//Edit these if default not satisfactory
	m_initializer.iWidth = SCREEN_WIDTH_WINDOWED;
	m_initializer.iHeight = SCREEN_HEIGHT_WINDOWED;

	/*m_initializer.iWidth = SCREEN_WIDTH_MAX;
	m_initializer.iHeight = SCREEN_HEIGHT_MAX;
	m_initializer.bIsWindowed = false;
	m_initializer.bVSYNC = false;*/

	
	m_scenes[0] = new MainMenuScene();
	m_scenes[1] = new GameplayScene();
}

/**
*	Shut down singleton
*	@author Serge Radinovich
*/
void GameRoot::shutDown()
{
	

	//Destroy all Root architecture (Mesh, Scene, DX9, Managers etc)
	IRoot::shutDown();

	for (auto& _rpScene : m_scenes)
		SAFE_DELETE(_rpScene);
}

/**
*	Destructor
*	@author Serge Radinovich
*/
GameRoot::~GameRoot()
{
}

/**
*	Initialize IRoot, pure virtual overridden
*	@author Serge Radinovich
*	@return	bool					True if initialization successful
*/
bool GameRoot::init()
{
	m_pPhysicsSystem->init(-SCREEN_BOUND, SCREEN_BOUND, -SCREEN_BOUND, SCREEN_BOUND);

	for (UINT i = 0; i < m_scenes.size(); ++i)
	{
		m_scenes[i]->init();
		if (i != m_eCurrentScene)
			m_scenes[i]->setActive(false);
	}



	return true;
}



/**
*	Create all the objects required for the game scene.
*	@author Serge Radinovich
*/
void GameRoot::createScene() //AB: Object Ordering
{

	m_pCamera->setPos(&D3DXVECTOR3(0.0f, 0.0f, -100.0f));
	m_pCamera->lookAt(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//Create Directional light
	m_parallelLightHandle = DX10Renderer::createLight(E_LIGHT_TYPE::LT_PARALLEL);
	//DX10Renderer::getLight(m_parallelLightHandle.first, m_parallelLightHandle.second).dir = D3DXVECTOR3(-0.57735f, -0.57735f, 0.57735f);


}


/**
*	Update loop called from IRoot, pure virtual overridden. Calls all other updates
*	@author Serge Radinovich
*	@param	_fDeltaTime				Delta Time
*/
void GameRoot::update(float _fDeltaTime)
{
	InputHandler::getInstance()->update();

	m_scenes[m_eCurrentScene]->update(_fDeltaTime);

	E_SCENE _eTempScene;
	if (m_scenes[m_eCurrentScene]->sceneEnding(_eTempScene))
	{
		m_scenes[m_eCurrentScene]->setActive(false);
		m_eCurrentScene = _eTempScene;
		m_scenes[m_eCurrentScene]->setActive(true);
	}

	/*D3DXMATRIX _rotMtx;
	D3DXMatrixRotationAxis(&_rotMtx, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DX_PI * _fDeltaTime);

	D3DXVECTOR3 _dir = DX10Renderer::getLight(m_parallelLightHandle.first, m_parallelLightHandle.second).dir;
	D3DXVECTOR4 _dirTemp;
	D3DXVec3Transform(&_dirTemp, &_dir, &_rotMtx);

	_dir.x = _dirTemp.x;
	_dir.y = _dirTemp.y;
	_dir.z = _dirTemp.z;
	DX10Renderer::getLight(m_parallelLightHandle.first, m_parallelLightHandle.second).dir = _dir;*/

}





