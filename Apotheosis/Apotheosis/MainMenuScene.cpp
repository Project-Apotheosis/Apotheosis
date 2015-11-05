#include "MainMenuScene.h"
#include <functional>

MainMenuScene::MainMenuScene(){}
MainMenuScene::~MainMenuScene(){}

void MainMenuScene::init() 
{

	m_buttons[0].init(b2Vec2(0, 5), 5, 2);
	m_buttons[0].addRenderable("StartGame_", ".png", 2);
	m_buttons[1].init(b2Vec2(0, -5), 5, 2);
	m_buttons[1].addRenderable("ExitGame_", ".png", 2);

	//Initialize buttons' executables
	vector<function<void()>> _callbacks;
	_callbacks.push_back([&](){ m_bSceneEnding = true; m_nextSceneSelected = S_GAMEPLAY; });

	//First button
	m_buttons.front().initExecutable(_callbacks);

	//Second button
	_callbacks.clear();
	_callbacks.push_back([&](){ m_bSceneEnding = true; m_nextSceneSelected = S_EXIT; });
	m_buttons.back().initExecutable(_callbacks);

	m_buttons.front().highlight(true); //Top button is automatically selected
}

void MainMenuScene::update(float _fDeltaTime) 
{
	for_each(m_buttons.begin(), m_buttons.end(), [&](MainMenuButton& _rButton){ _rButton.update(_fDeltaTime); });

	//for (auto& rButton : m_buttons)
	//	rButton.update(_fDeltaTime);


	//Handle input 
	if (InputHandler::getInstance()->handleGeneralButton(0, GB_DPAD_UP, _fDeltaTime))			//Navigate up
		navigateMenu(true);
	else if (InputHandler::getInstance()->handleGeneralButton(0, GB_DPAD_DOWN, _fDeltaTime))	//Navigate down
		navigateMenu(false);
	else if (InputHandler::getInstance()->handleGeneralButton(0, GB_A, _fDeltaTime))			//Click button
		m_buttons[m_iCurrentButtonIndex].click();

}

void MainMenuScene::navigateMenu(bool _bUp)
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


void MainMenuScene::setActive(bool _bActive) 
{
	for_each(m_buttons.begin(), m_buttons.end(), [&](MainMenuButton& _rButton){ _rButton.setActive(_bActive); });
	//for (auto& rButton : m_buttons)
	//	rButton.setActive(_bActive);
}

bool MainMenuScene::sceneEnding(E_SCENE& _reNextScene) 
{
	if (m_bSceneEnding)
	{
		_reNextScene = m_nextSceneSelected;
		m_bSceneEnding = false; //Reset
		return true;
	}
	return false;
}
