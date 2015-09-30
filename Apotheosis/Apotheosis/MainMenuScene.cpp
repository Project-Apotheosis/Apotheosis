#include "MainMenuScene.h"


bool MainMenuScene::s_bSceneEnding = false;
E_SCENE MainMenuScene::s_nextSceneSelected = S_GAMEPLAY;

MainMenuScene::MainMenuScene(){}
MainMenuScene::~MainMenuScene(){}

void MainMenuScene::init() 
{

	for (int i = 0; i < m_buttons.size(); ++i)
	{
		//rButton.initExecutable() //TODO
		m_buttons[i].init(b2Vec2(0, -i * 5), 5, 2);
		m_buttons[i].addRenderable("button", ".png", 2);
	}

	//Initialize buttons' executables
	vector<void(*)()> _callbacks;
	_callbacks.push_back(loadGameplayScene);

	for (auto& rButton : m_buttons)
		rButton.initExecutable(_callbacks);

	m_buttons.front().highlight(true); //Top button is automatically selected
}

void MainMenuScene::update(float _fDeltaTime) 
{
	for (auto& rButton : m_buttons)
		rButton.update(_fDeltaTime);


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
	for (auto& rButton : m_buttons)
		rButton.setActive(_bActive);
}

bool MainMenuScene::sceneEnding(E_SCENE& _reNextScene) 
{
	if (MainMenuScene::s_bSceneEnding)
	{
		_reNextScene = MainMenuScene::s_nextSceneSelected;
		MainMenuScene::s_bSceneEnding = false; //Reset
		return true;
	}
	return false;
}

void MainMenuScene::loadGameplayScene()
{
	MainMenuScene::s_bSceneEnding = true;
	MainMenuScene::s_nextSceneSelected = S_GAMEPLAY;
}