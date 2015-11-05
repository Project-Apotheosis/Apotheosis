// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : GameRoot.h
// Description :  GameRoot header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#pragma once

#ifndef __GAME_ROOT_H__
#define __GAME_ROOT_H__

#include "IRoot.h"
#include "Platform.h"
#include "Player.h"
#include "GameplayScene.h"
#include "MainMenuScene.h"




class GameRoot : public IRoot
{
public: //Construction
	GameRoot();
	~GameRoot();
	void shutDown()	override;

private: //Data

	E_SCENE m_eCurrentScene{ S_MAINMENU };
	array<IGameScene*, 2> m_scenes;

	pair<E_LIGHT_TYPE, unsigned> m_parallelLightHandle{};


private: //Internals

	bool init()								override;
	//Create all objects here

	void createScene()						override;

	//All application-specific updating happens here
	bool update(float _fDeltaTime)			override;

};

#endif	//__GAME_ROOT_H__