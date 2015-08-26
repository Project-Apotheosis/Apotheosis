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
	m_initializer.iWidth = 1600;
	m_initializer.iHeight = 1200;

	/*m_initializer.iWidth = SCREEN_WIDTH_MAX;
	m_initializer.iHeight = SCREEN_HEIGHT_MAX;
	m_initializer.bIsWindowed = false;
	m_initializer.bVSYNC = false;*/
}

/**
*	Shut down singleton
*	@author Serge Radinovich
*/
void GameRoot::shutDown()
{
	//Destroy all Root architecture (Mesh, Scene, DX9, Managers etc)
	IRoot::shutDown();
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
	m_pPhysicsSystem->init(-30, 30, -30, 30);

	return true;
}



/**
*	Create all the objects required for the game scene.
*	@author Serge Radinovich
*/
void GameRoot::createScene() //AB: Object Ordering
{
	for (UINT i = 0; i < 4; ++i)
		m_platforms[i].init(b2Vec2(-25.0f + 10.0f*i, -2.0f), 3.0f, 1.0f);

	m_player.init(b2Vec2(0.0f, 5.0f), 1.0f, 1.0f);
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
	
	for (auto& rPlatform : m_platforms)
		rPlatform.update(_fDeltaTime);

	m_player.update(_fDeltaTime);
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





