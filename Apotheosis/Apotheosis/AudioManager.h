// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2014 Media Design School 
// 
// File Name : AudioManager.cpp
// Description :  AudioManager cpp
// Author : HenryWinder
// Mail : henry@mcgredywinder.co.nz


#pragma once

#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "../FMOD api/inc/fmod.hpp"
#include "../FMOD api/inc/fmod_errors.h"



class AudioManager
{
	//Member functions
private: 
	//constructor
	AudioManager();
	static AudioManager* s_pInstance;
public:
	//constructors
	~AudioManager();

	//Singleton 	
	static AudioManager* getInstance()
	{
		if (!s_pInstance)
		{
			s_pInstance = new AudioManager();
		}

		return s_pInstance;
	}

	bool init();
	void shutDown();
	void Update();

	//sfx
	static void PlayJump();
	static void PlaySlide();
	static void PlayMelee();
	static void PlayRanged();
	static void PlayTakeDamage();

	//static void PlayWallSlide(); ?

	//music
	static void PlayMusic();
	static void PauseMusic();
	//volume control
	static void SetMusicVolume(float _fVolume);
	static void SetSFXVolume(float _fVolume);

	static float GetMusicVolume();
	static float GetSFXVolume();



protected:
private:

	//Member variables
public:
protected:
private:
	FMOD::System     *system;
	FMOD::ChannelGroup *channelMusic;
	FMOD::ChannelGroup *channelSFX;
	FMOD::Sound      *music, //music will be created using createStream
					*jump1, *jump2, *jump3, 
					*slide1, *slide2, *slide3,
					*melee1, *melee2, *melee3,
					*ranged1, *ranged2, *ranged3,
					*damage1, *damage2, *damage3;
	FMOD::Channel    *MusicChannel = 0, *SFXChannel = 0;
	FMOD_RESULT       result;
	int               key;
	unsigned int      version;
};

#endif