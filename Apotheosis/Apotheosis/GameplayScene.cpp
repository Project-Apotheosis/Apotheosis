#include "GameplayScene.h"
#include "LevelLoader.h"


GameplayScene::GameplayScene(){}
GameplayScene::~GameplayScene()
{
	sort(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform1, Platform& _rPlatform2){return !_rPlatform1.isActive() && _rPlatform2.isActive(); });
	//Save platforms to XML
	if (m_bSaveLevelOnExit)
		LevelLoader::toXML("GameplayScene.xml", { m_platforms, m_players });
}



void GameplayScene::init() 
{

	for (UINT i = 0; i < m_platforms.size(); ++i)
		m_platforms[i].init(b2Vec2(-25.0f + 10.0f*i, -2.0f), 3.0f, 1.0f);

	for (UINT i = 0; i < m_players.size(); ++i)
		m_players[i].init(b2Vec2(-2.0f + 2.0f*i, 5.0f), 1.0f, 1.0f);

}

void GameplayScene::update(float _fDeltaTime) 
{
	for_each(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform){ _rPlatform.update(_fDeltaTime); });
	//for (auto& rPlatform : m_platforms)
	//	rPlatform.update(_fDeltaTime);

	for_each(m_players.begin(), m_players.end(), [&](Player& _rPlayer){ _rPlayer.update(_fDeltaTime); });
	//for (auto& rPlayer : m_players)
	//	rPlayer.update(_fDeltaTime);


	//Level editing

	//Platform spawning
	//Left mouse click - spawn platform
	pair<float, float> _mouseCoords;
	if (m_iInactivePlatformCount > 0 && InputHandler::getInstance()->handleMouseClick(0, _fDeltaTime, _mouseCoords))
	{
		
		//for (UINT i = 0; i < m_iInactivePlatformCount; ++i)
		//{
			//if (!m_platforms[i].isActive())
			//{
				m_platforms.front().setActive(true);
				//NDC to world
				_mouseCoords.first *= SCREEN_BOUND;
				_mouseCoords.second *= SCREEN_BOUND;
				m_platforms.front().setPosition(_mouseCoords.first, _mouseCoords.second);
				m_iInactivePlatformCount--;
				sort(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform1, Platform& _rPlatform2){return !_rPlatform1.isActive() && _rPlatform2.isActive(); }); //Active ones go to the back
				//break;
			//}
		//}
	}
	//Right mouse click - remove platform
	else if (m_iInactivePlatformCount < m_platforms.size() && InputHandler::getInstance()->handleMouseClick(1, _fDeltaTime, _mouseCoords))
	{
		//Remove spawned platform
		
		//NDC to world
		_mouseCoords.first *= SCREEN_BOUND;
		_mouseCoords.second *= SCREEN_BOUND;
		int _iThreshold = m_iInactivePlatformCount > 0 ? m_iInactivePlatformCount - 1 : 0;
		for (UINT i = m_platforms.size() - 1; i > _iThreshold; --i)
		{

			if (fabs(m_platforms[i].getPosition().x - _mouseCoords.first) < 5.0f && fabs(m_platforms[i].getPosition().y - _mouseCoords.second) < 5.0f)
			{
				m_platforms[i].setActive(false);
				m_iInactivePlatformCount++;
				sort(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform1, Platform& _rPlatform2){return !_rPlatform1.isActive() && _rPlatform2.isActive(); }); //Active ones go to the back
				break;
			}

		}
	}
	
	//Player spawns
	int _iPlayerSpawnerIndex = -1; //Set to 0 - 3 if user clicks button this frame
	if (InputHandler::getInstance()->handleGeneralKB(DIK_1, _fDeltaTime))
	{
		//Set player 1 spawn
		_iPlayerSpawnerIndex = 0;
		
	}
	else if (InputHandler::getInstance()->handleGeneralKB(DIK_2, _fDeltaTime))
	{
		//Set player 2 spawn
		_iPlayerSpawnerIndex = 1;
	}
	else if (InputHandler::getInstance()->handleGeneralKB(DIK_3, _fDeltaTime))
	{
		//Set player 3 spawn
		_iPlayerSpawnerIndex = 2;
	}
	else if (InputHandler::getInstance()->handleGeneralKB(DIK_4, _fDeltaTime))
	{
		//Set player 4 spawn
		_iPlayerSpawnerIndex = 3;
	}
	if (_iPlayerSpawnerIndex != -1)
		m_players[_iPlayerSpawnerIndex].setSpawnPosition(InputHandler::getInstance()->mouseScreenCoordX() * SCREEN_BOUND, InputHandler::getInstance()->mouseScreenCoordY() * SCREEN_BOUND);


	//Restart gameplay scene
	if (InputHandler::getInstance()->handleGeneralKB(DIK_R, _fDeltaTime))
	{
		//Respawn players
		for (auto& rPlayer : m_players)
			rPlayer.respawn();
	}

}

void GameplayScene::setActive(bool _bActive) 
{
	for_each(m_players.begin(), m_players.end(), [&](Player& _rPlatform){ _rPlatform.setActive(_bActive); });
	
	if (!_bActive)
	{
		for_each(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform){ _rPlatform.setActive(false); });
		
	}
	else
	{
		m_bSaveLevelOnExit = true; //Scene was loaded, so we will save level 
		//Load platforms from XML	
		LevelLoader::fromXML("GameplayScene.xml", { m_platforms, m_players });
		sort(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform1, Platform& _rPlatform2){return !_rPlatform1.isActive() && _rPlatform2.isActive(); });
		for (auto& platform : m_platforms)
			if (!platform.isActive())
				m_iInactivePlatformCount++;

		//Respawn players
		for (auto& rPlayer : m_players)
			rPlayer.respawn();
	}
	

	//for (auto& rPlatform : m_platforms)
	//	rPlatform.setActive(_bActive);


}

bool GameplayScene::sceneEnding(E_SCENE& _reNextScene)
{
	if (m_bSceneEnding)
	{
		_reNextScene = m_nextSceneSelected;
		m_bSceneEnding = false; //Reset
		return true;
	}
	return false;
}
