#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "Util.h"
#include "IGameScene.h"
#include "IButton.h"
#include "InputHandler.h"
#include "MainMenuButton.h"

class MainMenuScene : public IGameScene
{

public: //Construction

	MainMenuScene();
	~MainMenuScene();

	MainMenuScene(const MainMenuScene&) = delete;
	MainMenuScene& operator=(const MainMenuScene&) = delete;

private: //Data

	array<MainMenuButton, 2> m_buttons;
	UINT m_iCurrentButtonIndex{};



public: //Interface

	void init() override;

	void update(float _fDeltaTime) override;

	void navigateMenu(bool _bUp);


	void setActive(bool _bActive) override;

	bool sceneEnding(E_SCENE& _reNextScene) override;


};




#endif //__MAIN_MENU_SCENE_H__