// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2014 Media Design School 
// 
// File Name : XInputManager.cpp
// Description :  XInputManager cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com


#include "XInputManager.h"

#pragma comment(lib, "Xinput.lib")


	// XInput Button values
WORD XInputManager::XINPUT_Buttons[14] = {
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y,
		XINPUT_GAMEPAD_DPAD_UP,
		XINPUT_GAMEPAD_DPAD_DOWN,
		XINPUT_GAMEPAD_DPAD_LEFT,
		XINPUT_GAMEPAD_DPAD_RIGHT,
		XINPUT_GAMEPAD_LEFT_SHOULDER,
		XINPUT_GAMEPAD_RIGHT_SHOULDER,
		XINPUT_GAMEPAD_LEFT_THUMB,
		XINPUT_GAMEPAD_RIGHT_THUMB,
		XINPUT_GAMEPAD_START,
		XINPUT_GAMEPAD_BACK
	};


XInputManager::XInputManager()
{
}

	// Overloaded constructor
XInputManager::XInputManager(int _iIndex)
	{
		// Set gamepad index
		m_iGamePadIndex = _iIndex;

		// Iterate through all gamepad buttons
		for (int i = 0; i < s_iButtonCount; i++)
		{
			m_bPrev_ButtonStates[i] = false;
			m_bButtonStates[i] = false;
			m_bGamepad_ButtonsDown[i] = false;
		}
	}




XINPUT_STATE XInputManager::getState()
	{
		// Temporary XINPUT_STATE to return
		XINPUT_STATE GamepadState;

		// Zero memory
		ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

		// Get the state of the gamepad
		XInputGetState(m_iGamePadIndex, &GamepadState);

		// Return the gamepad state
		return GamepadState;
	}


bool XInputManager::isConnected()
	{
		// Zero memory
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));

		// Get the state of the gamepad
		DWORD Result = XInputGetState(m_iGamePadIndex, &m_state);

		if (Result == ERROR_SUCCESS)
			return true;  // The gamepad is connected
		else
			return false; // The gamepad is not connected
	}

void XInputManager::poll()
	{
		m_state = getState();

		// Iterate through all gamepad buttons
		for (int i = 0; i < s_iButtonCount; i++)
		{
			// Set button state for current frame
			m_bButtonStates[i] = (m_state.Gamepad.wButtons &
				XINPUT_Buttons[i]) == XINPUT_Buttons[i];

			// Set 'DOWN' state for current frame
			m_bGamepad_ButtonsDown[i] = !m_bPrev_ButtonStates[i] &&
				m_bButtonStates[i];
		}
	}

	// Update button states for next frame
void XInputManager::refreshState()
	{
		memcpy(m_bPrev_ButtonStates, m_bButtonStates,
			sizeof(m_bPrev_ButtonStates));
	}

	// Deadzone check for Left Thumbstick
bool XInputManager::lStick_InDeadzone()
	{
		// Obtain the X & Y axes of the stick
		short sX = m_state.Gamepad.sThumbLX;
		short sY = m_state.Gamepad.sThumbLY;

		// X axis is outside of deadzone
		if (sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return false;

		// Y axis is outside of deadzone
		if (sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return false;

		// One (or both axes) axis is inside of deadzone
		return true;
	}

	// Deadzone check for Right Thumbstick
bool XInputManager::rStick_InDeadzone()
	{
		// Obtain the X & Y axes of the stick
		short sX = m_state.Gamepad.sThumbRX;
		short sY = m_state.Gamepad.sThumbRY;

		// X axis is outside of deadzone
		if (sX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
			sX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return false;

		// Y axis is outside of deadzone
		if (sY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
			sY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return false;

		// One (or both axes) axis is inside of deadzone
		return true;
	}

	// Return X axis of left stick
float XInputManager::leftStick_X()
	{
		if (lStick_InDeadzone())
			return 0.0f;

		// Obtain X axis of left stick
		short sX = m_state.Gamepad.sThumbLX;

		// Return axis value, converted to a float
		return (static_cast<float>(sX) / 32768.0f);
	}

	// Return Y axis of left stick
float XInputManager::leftStick_Y()
	{
		if (lStick_InDeadzone())
			return 0.0f;

		// Obtain Y axis of left stick
		short sY = m_state.Gamepad.sThumbLY;

		// Return axis value, converted to a float
		return (static_cast<float>(sY) / 32768.0f);
	}

	// Return X axis of right stick
float XInputManager::rightStick_X()
	{
		if (rStick_InDeadzone())
			return 0.0f;

		// Obtain X axis of right stick
		short sX = m_state.Gamepad.sThumbRX;

		// Return axis value, converted to a float
		return (static_cast<float>(sX) / 32768.0f);
	}

	// Return Y axis of right stick
float XInputManager::rightStick_Y()
	{
		if (rStick_InDeadzone())
			return 0.0f;
		// Obtain the Y axis of the left stick
		short sY = m_state.Gamepad.sThumbRY;

		// Return axis value, converted to a float
		return (static_cast<float>(sY) / 32768.0f);
	}

	// Return value of left trigger
float XInputManager::leftTrigger()
	{
		// Obtain value of left trigger
		BYTE Trigger = m_state.Gamepad.bLeftTrigger;

		if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			return Trigger / 255.0f;

		return 0.0f; // Trigger was not pressed
	}

	// Return value of right trigger
float XInputManager::rightTrigger()
	{
		// Obtain value of right trigger
		BYTE Trigger = m_state.Gamepad.bRightTrigger;

		if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			return Trigger / 255.0f;

		return 0.0f; // Trigger was not pressed
	}

	// Vibrate the gamepad (values of 0.0f to 1.0f only)
void XInputManager::rumble(float _fLeftMotor, float _fRightMotor)
	{
		// Vibration state
		XINPUT_VIBRATION VibrationState;

		// Zero memory
		ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

		// Calculate vibration values
		int iLeftMotor = int(_fLeftMotor * 65535.0f);
		int iRightMotor = int(_fRightMotor * 65535.0f);

		// Set vibration values
		VibrationState.wLeftMotorSpeed = iLeftMotor;
		VibrationState.wRightMotorSpeed = iRightMotor;

		// Set the vibration state
		XInputSetState(m_iGamePadIndex, &VibrationState);
	}

bool XInputManager::getButtonPressed(E_GAMEPAD_BUTTON _eButton)
	{
		if (m_state.Gamepad.wButtons & XINPUT_Buttons[_eButton])
		{
			return true; // The button is pressed
		}

		return false; // The button is not pressed
	}

bool XInputManager::getButtonDown(E_GAMEPAD_BUTTON _eButton)
	{
		return m_bGamepad_ButtonsDown[_eButton];
	}

