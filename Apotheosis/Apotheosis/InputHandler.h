// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2014 Media Design School 
// 
// File Name : InputHandler.h
// Description :  InputHandler header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#pragma once

#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__


#include "XInputManager.h"
#include "D3DInputManager.h"

class InputHandler 
{
private: //Construction

	//Ctor private
	InputHandler();
	static InputHandler* s_pInstance;

public: //Interface

	void init(HINSTANCE, HWND);
	void shutDown();
	~InputHandler();

	//Singleton 
	static InputHandler* getInstance()
	{
		if (!s_pInstance)
			s_pInstance = new InputHandler();		

		return s_pInstance;
	}

	//No copying
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;

private: //Data

	XInputManager* m_pGamePad{}; //Xbox 360 controller
	D3DInputManager* m_pDinput{};
public: //Interface


	bool handleObjectTranslation(D3DXVECTOR2& _rTranslate, float _fDeltaTime);
	bool handleJump(float _fDeltaTime);
	bool handleShoot(float _fDeltaTime);

	void update();
};

#endif //__INPUT_HANDLER_H__