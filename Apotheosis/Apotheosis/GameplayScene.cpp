#include "GameplayScene.h"
#include "LevelLoader.h"
#include "AudioManager.h"


GameplayScene::GameplayScene(){}
GameplayScene::~GameplayScene()
{
	sort(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform1, Platform& _rPlatform2){return !_rPlatform1.isActive() && _rPlatform2.isActive(); });
	//Save platforms to XML
	//if (m_bSaveLevelOnExit)
		//LevelLoader::toXML("GameplayScene.xml", { m_platforms, m_players });
}



void GameplayScene::init() 
{
	m_fSleep = 0.0f;

	for (UINT i = 0; i < m_platforms.size(); ++i)
		m_platforms[i].init(b2Vec2(-25.0f + 10.0f*i, -2.0f), 3.0f, 3.0f);

	for (UINT i = 0; i < m_players.size(); ++i)
	{
		m_players[i].init( b2Vec2(-2.0f + 2.0f*i, 5.0f), 2.0f, 2.0f);
		m_players[i].initCallbacks([&](Player& _rkPlayer){this->playerAttackCallback(_rkPlayer); },
								   [&](){this->playerChannelSuccessCallback(); });
	}
		
	m_buttons[0].init(b2Vec2(0, 5), 5, 2);
	m_buttons[0].addRenderable("ResumeGame_", ".png", 2);
	m_buttons[1].init(b2Vec2(0, -5), 5, 2);
	m_buttons[1].addRenderable("ExitGame_", ".png", 2);


	//Return to game
	vector<function<void()>> _returnCallback;
	_returnCallback.push_back([&]() { this->changePauseState(false); });
	m_buttons[0].initExecutable(_returnCallback);

	//Exit to menu callback
	vector<function<void()>> _callbacks;
	_callbacks.push_back([&](){ m_bSceneEnding = true; m_nextSceneSelected = S_MAINMENU; });
	m_buttons[1].initExecutable(_callbacks);


	changePauseState(false);
}

void GameplayScene::changePauseState(bool _bPaused)
{
	m_bPaused = _bPaused;

	for (auto& rButton : m_buttons)
		rButton.setActive(_bPaused);

	for (auto& rPlatform : m_platforms)
		rPlatform.setActive(!_bPaused);

	for (auto& rPlayer : m_players)
		rPlayer.setActive(!_bPaused);

}



void GameplayScene::update(float _fDeltaTime) 
{
	if (m_fSleep > 0.0f)
	{
		m_fSleep -= _fDeltaTime;
	}
	for (auto& rPlayer : m_players)
	{
		rPlayer.setAllowInput(m_fSleep <= 0.0f);
	}

	if (m_bPaused)
	{
		for (auto& rButton : m_buttons)
			rButton.update(_fDeltaTime);

		//Handle menu input 
		if (InputHandler::getInstance()->handleGeneralButton(0, GB_DPAD_UP, _fDeltaTime))			//Navigate up
		{
			navigateMenu(true);
		}
		else if (InputHandler::getInstance()->handleGeneralButton(0, GB_DPAD_DOWN, _fDeltaTime))	//Navigate down
		{
			navigateMenu(false);
		}
		else if (InputHandler::getInstance()->handleGeneralButton(0, GB_A, _fDeltaTime))			//Click button
		{
			m_buttons[m_iCurrentButtonIndex].click();
		}
		else if (InputHandler::getInstance()->handleGeneralButton(0, GB_START, _fDeltaTime)) //Unpause
		{
			changePauseState(false);
		}
	}
	else
	{
		//Pause
		if (InputHandler::getInstance()->handleGeneralButton(0, GB_START, _fDeltaTime))
		{
			changePauseState(true);
			m_iCurrentButtonIndex = 1;
			navigateMenu(true);
		}

		for_each(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform){ _rPlatform.update(_fDeltaTime); });
		//for (auto& rPlatform : m_platforms)
		//	rPlatform.update(_fDeltaTime);

		for_each(m_players.begin(), m_players.end(), [&](Player& _rPlayer){ _rPlayer.update(_fDeltaTime); });
		//for (auto& rPlayer : m_players)
		//	rPlayer.update(_fDeltaTime);

		//Check for win state
		for (auto& rPlayer : m_players)
		{
			if (rPlayer.isWinner())
			{
				m_bSceneEnding = true; 
				m_nextSceneSelected = S_MAINMENU;
				return;
			}
		}


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
}

void GameplayScene::setActive(bool _bActive) 
{
	for_each(m_players.begin(), m_players.end(), [&](Player& _rPlatform){ _rPlatform.setActive(_bActive); });
	
	if (!_bActive)
	{
		for_each(m_platforms.begin(), m_platforms.end(), [&](Platform& _rPlatform){ _rPlatform.setActive(false); });

		//Respawn players
		for (auto& rPlayer : m_players)
		{
			rPlayer.resetScore();
		}
		
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

		AudioManager::PlayGong();

		changePauseState(false);
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
		m_fSleep = 0.0f;
		return true;
	}
	return false;
}

void GameplayScene::playerChannelSuccessCallback()
{
	for (auto& rPlayer : m_players)
	{
		rPlayer.respawn();
	}
	//New round has started, play sound for start of new round and pause input for 2 seconds
	AudioManager::PlayGong();
	m_fSleep = 2.0f;
}


void GameplayScene::playerAttackCallback(Player& _rkAttackingPlayer)
{
	for (auto& rPlayerAttacked : m_players)
	{
		if (rPlayerAttacked.getPlayerID() != _rkAttackingPlayer.getPlayerID())
		{
			b2Vec2 distVec = rPlayerAttacked.getPosition() - _rkAttackingPlayer.getPosition();
			if (distVec.LengthSquared() < 30.0f)
			{				
				_rkAttackingPlayer.stealSouls(rPlayerAttacked.getSoulCount());
				rPlayerAttacked.kill(distVec);
				return;
			}
		}
	}
}


void GameplayScene::navigateMenu(bool _bUp)
{
	if (_bUp)
	{
		if (m_iCurrentButtonIndex == 0)
			return;

		m_buttons[m_iCurrentButtonIndex--].highlight(false);
		m_buttons[m_iCurrentButtonIndex].highlight(true);

	}
	else
	{
		if (m_iCurrentButtonIndex == m_buttons.size() - 1)
			return;

		m_buttons[m_iCurrentButtonIndex++].highlight(false);
		m_buttons[m_iCurrentButtonIndex].highlight(true);
	}

}