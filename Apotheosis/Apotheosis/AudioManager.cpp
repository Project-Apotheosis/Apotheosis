// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2014 Media Design School 
// 
// File Name : AudioManager.h
// Description :  AudioManager header
// Author : HenryWinder
// Mail : henry@mcgredywinder.co.nz

#include "AudioManager.h"

bool ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		return true;
	}
	return false;
}

//AudioManager* AM = AudioManager::getInstance();

AudioManager* AudioManager::s_pInstance = nullptr; ///Singleton pointer

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{

}

bool AudioManager::init()
{
	result = FMOD::System_Create(&system);
	if (ERRCHECK(result))
		return false;

	result = system->getVersion(&version);
	if (ERRCHECK(result))
		return false;

	if (version < FMOD_VERSION)
	{
		//the fmod version you are using is out of date
		return false;
	}


	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS caps;
	int numDrivers;

	// Get number of sound cards 
	result = system->getNumDrivers(&numDrivers);
	if (ERRCHECK(result))
		return false;

	// No sound cards (disable sound)
	if (numDrivers == 0)
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		if (ERRCHECK(result))
			return false;
	}

	// Get the capabilities of the default (0) sound card
	result = system->getDriverCaps(0, &caps, 0, &speakerMode);
	if (ERRCHECK(result))
		return false;

	// Set the speaker mode to match that in Control Panel
	result = system->setSpeakerMode(speakerMode);
	if (ERRCHECK(result))
		return false;

	//First param is the max number of channels
	//If we play more than the specified number of sounds at a time then channels will be cut off for new sounds
	result = system->init(100, FMOD_INIT_NORMAL, 0);

	// If the selected speaker mode isn't supported by this sound card, switch it back to stereo
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if (ERRCHECK(result))
			return false;

		result = system->init(100, FMOD_INIT_NORMAL, 0);
	}
	if (ERRCHECK(result))
		return false;

	//now that FMOD has been initialized properly with the right correct system settings we can load our sounds into it
	//We will use createStream() for music and createSound() for SFX
	//Both of these use the same playSound() function

	if (ERRCHECK(system->createChannelGroup(NULL, &channelMusic)))
		return false;
	if(ERRCHECK(system->createChannelGroup(NULL, &channelSFX)))
		return false;

	SFXChannel->setChannelGroup(channelSFX);
	MusicChannel->setChannelGroup(channelMusic);

	//load jump sounds
	result = system->createSound("../Audio/Jump1.wav", FMOD_DEFAULT, 0, &jump1);
	if (ERRCHECK(result))
		return false;

	result = system->createSound("../Audio/Jump2.wav", FMOD_DEFAULT, 0, &jump2);
	if (ERRCHECK(result))
		return false;

	result = system->createSound("../Audio/Jump3.wav", FMOD_DEFAULT, 0, &jump3);
	if (ERRCHECK(result))
		return false;

	//load slide sounds
	//load melee sounds
	//load ranged sounds
	//load damage sounds

	return true;
}

void AudioManager::shutDown()
{
	//release jump sounds
	jump1->release();
	jump2->release();
	jump3->release();

	//release slide sounds
	//release melee sounds
	//release ranged sounds
	//release damage sounds

	system->close();
	system->release();
}

void AudioManager::Update()
{
	system->update();
}

void AudioManager::PlayJump()
{
	int i = rand() % 3 + 1;

	if (i == 1)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->jump1, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else if (i == 2)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->jump2, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->jump3, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
}

void AudioManager::PlaySlide()
{
	int i = rand() % 3 + 1;

	if (i == 1)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->slide1, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else if (i == 2)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->slide2, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->slide3, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
}

void AudioManager::PlayMelee()
{
	int i = rand() % 3 + 1;

	if (i == 1)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->melee1, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else if (i == 2)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->melee2, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->melee3, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
}

void AudioManager::PlayRanged()
{
	int i = rand() % 3 + 1;

	if (i == 1)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->ranged1, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else if (i == 2)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->ranged2, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->ranged3, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
}

void AudioManager::PlayTakeDamage()
{
	int i = rand() % 3 + 1;

	if (i == 1)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->damage1, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else if (i == 2)
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->damage2, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
	else
	{
		s_pInstance->result = s_pInstance->system->playSound(FMOD_CHANNEL_FREE, s_pInstance->damage3, false, &s_pInstance->SFXChannel);
		s_pInstance->SFXChannel->setChannelGroup(s_pInstance->channelSFX);
		s_pInstance->SFXChannel->setPaused(false);
	}
}

void AudioManager::SetMusicVolume(float _fVolume)
{
	s_pInstance->channelMusic->setVolume(_fVolume);
}

void AudioManager::SetSFXVolume(float _fVolume)
{
	s_pInstance->channelSFX->setVolume(_fVolume);
}

float AudioManager::GetMusicVolume()
{
	float fReturn;
	s_pInstance->channelMusic->getVolume(&fReturn);

	return fReturn;
}

float AudioManager::GetSFXVolume()
{
	float fReturn;
	s_pInstance->channelSFX->getVolume(&fReturn);

	return fReturn;
}