#include "GameplayScene.h"

bool GameplayScene::s_bSceneEnding = false;
E_SCENE GameplayScene::s_nextSceneSelected = S_GAMEPLAY;


GameplayScene::GameplayScene(){}
GameplayScene::~GameplayScene(){}


void GameplayScene::init() 
{

	for (UINT i = 0; i < 4; ++i)
		m_platforms[i].init(b2Vec2(-25.0f + 10.0f*i, -2.0f), 3.0f, 1.0f);

	for (UINT i = 0; i < m_players.size(); ++i)
		m_players[i].init(b2Vec2(-2.0f + 2.0f*i, 5.0f), 1.0f, 1.0f);

}

void GameplayScene::update(float _fDeltaTime) 
{
	for (auto& rPlatform : m_platforms)
		rPlatform.update(_fDeltaTime);

	for (auto& rPlayer : m_players)
		rPlayer.update(_fDeltaTime);
}

void GameplayScene::setActive(bool _bActive) 
{
	for (auto& rPlatform : m_platforms)
		rPlatform.setActive(_bActive);

	for (auto& rPlayer : m_players)
		rPlayer.setActive(_bActive);
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