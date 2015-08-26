// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2014 Media Design School 
// 
// File Name : D3DInputManager.cpp
// Description :  D3DInputManager cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "D3DInputManager.h"

	
	D3DInputManager::D3DInputManager()
		: m_pDI(nullptr)
		, m_pMouse(nullptr)
		, m_pKeyboard(nullptr)
	{
		ZeroMemory(&m_hWnd, sizeof(HWND));

	}

	D3DInputManager::~D3DInputManager()
	{
		if (m_pDI)
		{
			if (m_pKeyboard)
			{
				// Always unacquire device before calling Release(). 
				m_pKeyboard->Unacquire();
				m_pKeyboard->Release();
				m_pKeyboard = nullptr;
			}

			if (m_pMouse)
			{
				// Always unacquire device before calling Release(). 
				m_pMouse->Unacquire();
				m_pMouse->Release();
				m_pMouse = nullptr;
			}
			m_pDI->Release();
			m_pDI = nullptr;
		}
	}


	//Initialize direct input for use with mouse and keyboard
	bool D3DInputManager::init(HINSTANCE _hInst, HWND _hWnd)
	{
		m_hWnd = _hWnd;

		ZeroMemory(m_cKeyboardState, sizeof(m_cKeyboardState));
		ZeroMemory(&m_mouseState, sizeof(m_mouseState));

		//Create the DirectInput interface associated with the app instance 
		HR(DirectInput8Create(_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, 0));

		//Create a keyboard device associated with the DirectInput interface
		HR(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL));
		HR(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard));
		HR(m_pKeyboard->SetCooperativeLevel(_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND));

		//Create a mouse device associated with the DirectInput interface
		HR(m_pDI->CreateDevice(GUID_SysMouse, (LPDIRECTINPUTDEVICEW*)&m_pMouse, NULL));
		HR(m_pMouse->SetDataFormat(&c_dfDIMouse));
		HR(m_pMouse->SetCooperativeLevel(_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)); //DISCL_NONEXCLUSIVE | DISCL_FOREGROUND


		HRESULT _hResult = m_pKeyboard->Acquire();
		while (_hResult == E_ACCESSDENIED)
		{
			Sleep(10);
			_hResult = m_pKeyboard->Acquire();
		}
		_hResult = m_pMouse->Acquire();
		while (_hResult == E_ACCESSDENIED)
		{
			Sleep(10);
			_hResult = m_pMouse->Acquire();
		}

		return true;
	}


	//Retrieve current user input device
	//Called every frame by Input Handler
	void D3DInputManager::pollMouse()
	{
		HRESULT hr;

		hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);

		// check the return state to see if the device is still accessible
		if (FAILED(hr))
		{
			// try and reacquire the input device
			hr = m_pMouse->Acquire();
			// do a continuous loop until the device is reacquired
			while (hr == DIERR_INPUTLOST)
				hr = m_pMouse->Acquire();
		}
	}
	void D3DInputManager::pollKeyboard()
	{
		HRESULT hr;

		hr = m_pKeyboard->GetDeviceState(sizeof(m_cKeyboardState), (LPVOID)&m_cKeyboardState);

		if (FAILED(hr))
		{
			// try and reacquire the input device
			hr = m_pKeyboard->Acquire();
			// do a continuous loop until the device is reacquired
			while (hr == DIERR_INPUTLOST)
				hr = m_pKeyboard->Acquire();
		}
	}

	//Mouse button
	bool D3DInputManager::isMouseButtonDown(int _iID)
	{
		//return static_cast<bool>(BUTTONDOWN(m_mouseState, _iID));
		return (m_mouseState.rgbButtons[_iID] & 0x80) != 0;
	}

	//Keyboard button
	bool D3DInputManager::isKeyDown(char _cKey)
	{
		return (m_cKeyboardState[_cKey] & 0x80) != 0;
	}

