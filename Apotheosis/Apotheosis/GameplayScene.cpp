#include "GameplayScene.h"
#include "LevelLoader.h"

bool GameplayScene::s_bSceneEnding = false;
E_SCENE GameplayScene::s_nextSceneSelected = S_GAMEPLAY;


GameplayScene::GameplayScene(){}
GameplayScene::~GameplayScene()
{
	sort(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform1, Platform& _rPlatform2){return !_rPlatform1.isActive() && _rPlatform2.isActive(); });
	LevelLoader::toXML("GameplayScene.xml", m_platforms);
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
	for_each(m_platforms.begin(), m_platforms.end(), [&, _fDeltaTime](Platform& _rPlatform){ _rPlatform.update(_fDeltaTime); });
	//for (auto& rPlatform : m_platforms)
	//	rPlatform.update(_fDeltaTime);

	for_each(m_players.begin(), m_players.end(), [&, _fDeltaTime](Player& _rPlayer){ _rPlayer.update(_fDeltaTime); });
	//for (auto& rPlayer : m_players)
	//	rPlayer.update(_fDeltaTime);


	//Level editing
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
}

void GameplayScene::setActive(bool _bActive) 
{
	for_each(m_players.begin(), m_players.end(), [&, _bActive](Player& _rPlatform){ _rPlatform.setActive(_bActive); });
	
	if (!_bActive)
	{
		for_each(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform){ _rPlatform.setActive(false); });
		
	}
	else
	{
		//Load platforms from XML	
		LevelLoader::fromXML("GameplayScene.xml", m_platforms);
		sort(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform1, Platform& _rPlatform2){return !_rPlatform1.isActive() && _rPlatform2.isActive(); });
		for (auto& platform : m_platforms)
			if (!platform.isActive())
				m_iInactivePlatformCount++;
	}
	

	//for (auto& rPlatform : m_platforms)
	//	rPlatform.setActive(_bActive);

	//for (auto& rPlayer : m_players)
	//	rPlayer.setActive(_bActive);
}

bool GameplayScene::sceneEnding(E_SCENE& _reNextScene)
{
	if (GameplayScene::s_bSceneEnding)
	{
		_reNextScene = GameplayScene::s_nextSceneSelected;
		GameplayScene::s_bSceneEnding = false; //Reset
		return true;
	}
	return false;
}

void GameplayScene::loadMenuScene()
{
	assert(false && "Gameplay to menu scene transition not implemented");
	GameplayScene::s_bSceneEnding = true;
	GameplayScene::s_nextSceneSelected = S_GAMEPLAY;
}