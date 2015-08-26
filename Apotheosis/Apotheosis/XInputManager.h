// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2014 Media Design School 
// 
// File Name : XInputManager.h
// Description :  XInputManager header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#ifndef ___XInputManager_CONTROLLER_H__
#define ___XInputManager_CONTROLLER_H__

#include <Windows.h>
#include "XInput.h"
#include "Util.h"



	enum E_GAMEPAD_BUTTON : int
	{
		GB_A = 0,
		GB_B,
		GB_X,
		GB_Y,
		GB_DPAD_UP,
		GB_DPAD_DOWN,
		GB_DPAD_LEFT,
		GB_DPAD_RIGHT,
		GB_L_SHOULDER,
		GB_R_SHOULDER,
		GB_START,
		GB_BACK
	};


	class XInputManager
	{
	public: //Construction

		XInputManager();
		XInputManager(int _iIndex);

	private: //Data

		XINPUT_STATE m_state;
		int m_iGamePadIndex;

		static const int s_iButtonCount = 14;    // Total gamepad buttons
		bool m_bPrev_ButtonStates[s_iButtonCount]; // Previous frame button states
		bool m_bButtonStates[s_iButtonCount];      // Current frame button states

		// Buttons pressed on current frame
		bool m_bGamepad_ButtonsDown[s_iButtonCount];

	public: //Interface

		XINPUT_STATE getState();
		int getIndex() const { return m_iGamePadIndex; }
		bool isConnected();
		void poll();
		void refreshState();
		// Thumbstick functions
		// - Return true if stick is inside deadzone, false if outside
		bool lStick_InDeadzone();
		bool rStick_InDeadzone();

		float leftStick_X();  // Return X axis of left stick
		float leftStick_Y();  // Return Y axis of left stick
		float rightStick_X(); // Return X axis of right stick
		float rightStick_Y(); // Return Y axis of right stick

		// Trigger functions
		float leftTrigger();  // Return value of left trigger
		float rightTrigger(); // Return value of right trigger

		// Vibrate the gamepad (0.0f cancel, 1.0f max speed)
		void rumble(float _fLeftMotor = 0.0f, float _fRightMotor = 0.0f);

		bool getButtonPressed(E_GAMEPAD_BUTTON _eButton);
		bool getButtonDown(E_GAMEPAD_BUTTON _eButton);

	public: //Statics

		// XInput Button values
		static WORD XINPUT_Buttons[14];


	};



#endif //___XInputManager_CONTROLLER_H__

