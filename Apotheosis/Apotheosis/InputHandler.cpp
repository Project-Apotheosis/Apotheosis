// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2014 Media Design School 
// 
// File Name : InputHandler.cpp
// Description :  InputHandler cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "InputHandler.h"

InputHandler* InputHandler::s_pInstance = nullptr;


/**
*	Constructor
*	@author Serge Radinovich
*/
InputHandler::InputHandler()
{
	for (UINT i = 0; i < m_pGamePads.size(); ++i)
		m_pGamePads[i] = new XInputManager(i);

	m_pDinput = new D3DInputManager();
}

void InputHandler::init(HINSTANCE _hInst, HWND _hWND)
{
	m_pDinput->init(_hInst, _hWND);
}



/**
*	Destructor
*	@author Serge Radinovich
*/
InputHandler::~InputHandler()
{
}


/**
*	Shut down singleton
*	@author Serge Radinovich
*/
void InputHandler::shutDown()
{
	SAFE_DELETE(m_pDinput);

	for (UINT i = 0; i < m_pGamePads.size(); ++i)
		SAFE_DELETE(m_pGamePads[i]);

	delete s_pInstance;
}


/**
*	Handle shooting in game
*	@author Serge Radinovich
*	@param	_fDeltaTime				Delta Time
*	@return	bool					True if input detected
*/
bool InputHandler::handleShoot(UINT _iPlayerID, float _fDeltaTime)
{
	static float s_fLastHandle = 0.0f;
	static float s_fTimeElapsed = 0.0f;

	s_fTimeElapsed += _fDeltaTime;

	if (s_fTimeElapsed - s_fLastHandle > 0.2f)
	{
		/*if (isKeyDown(DIK_SPACE))
		{
			s_fLastHandle = s_fTimeElapsed;
			return true;
		}*/
	}
	return false;
}

bool InputHandler::handleJump(UINT _iPlayerID, float _fDeltaTime)
{
	m_inputTimersJump[_iPlayerID].first += _fDeltaTime;

	if (m_inputTimersJump[_iPlayerID].first - m_inputTimersJump[_iPlayerID].second > 0.56f)
	{
		if (m_pGamePads[_iPlayerID]->getButtonPressed(E_GAMEPAD_BUTTON::GB_A) /*|| m_pDinput->isKeyDown(DIK_SPACE)*/)
		{
			m_inputTimersJump[_iPlayerID].second = m_inputTimersJump[_iPlayerID].first;
			return true;
		}
	}
	return false;
}


bool InputHandler::handleDash(UINT _iPlayerID, float _fDeltaTime, int& _rOutDirection)
{
	_rOutDirection = 0;
	m_inputTimersDash[_iPlayerID].first += _fDeltaTime;

	if (m_inputTimersDash[_iPlayerID].first - m_inputTimersDash[_iPlayerID].second > 0.5f)
	{
		
		if (m_pGamePads[_iPlayerID]->getButtonPressed(E_GAMEPAD_BUTTON::GB_L_SHOULDER))
		{
			m_inputTimersDash[_iPlayerID].second = m_inputTimersDash[_iPlayerID].first;
			_rOutDirection = -1;
			return true;
		}
		else if (m_pGamePads[_iPlayerID]->getButtonPressed(E_GAMEPAD_BUTTON::GB_R_SHOULDER))
		{
			m_inputTimersDash[_iPlayerID].second = m_inputTimersDash[_iPlayerID].first;
			_rOutDirection = 1;
			return true;
		}
	}
	return false;
}

bool InputHandler::handleAttack(UINT _iPlayerID, float _fDeltaTime)
{
	m_inputTimersAttack[_iPlayerID].first += _fDeltaTime;

	if (m_inputTimersAttack[_iPlayerID].first - m_inputTimersAttack[_iPlayerID].second > 1.0f)
	{
		if (m_pGamePads[_iPlayerID]->getButtonPressed(E_GAMEPAD_BUTTON::GB_X))
		{
			m_inputTimersAttack[_iPlayerID].second = m_inputTimersAttack[_iPlayerID].first;
			return true;
		}
	}
	return false;
}


/**
*	Handle Object translation
*	@author Serge Radinovich
*	@param	_rTranslate				Return new forward vec to translate by
*	@param	_fDeltaTime				Delta Time
*	@return	bool					True if input detected
*/
bool InputHandler::handleObjectTranslation(UINT _iPlayerID, D3DXVECTOR2& _rTranslate, float _fDeltaTime)
{

	m_inputTimersMove[_iPlayerID].first += _fDeltaTime;

	if (m_inputTimersMove[_iPlayerID].first - m_inputTimersMove[_iPlayerID].second > 0.1f)
	{

		bool _bMoveHandled = false;

		//Vertical
		if (m_pGamePads[_iPlayerID]->leftStick_Y() > 0.1f /*|| m_pDinput->isKeyDown(DIK_W)*/)
		{
			_rTranslate += D3DXVECTOR2(0, 1);
			_bMoveHandled = true;
		}
		else if (m_pGamePads[_iPlayerID]->leftStick_Y() < -0.1f /*|| m_pDinput->isKeyDown(DIK_S)*/)
		{
			_rTranslate += D3DXVECTOR2(0, -1);
			_bMoveHandled = true;
		}

		//Horizontal
		if (m_pGamePads[_iPlayerID]->leftStick_X() < -0.1f /*|| m_pDinput->isKeyDown(DIK_A)*/)
		{
			_rTranslate += D3DXVECTOR2(-1, 0);
			_bMoveHandled = true;
		}
		else if (m_pGamePads[_iPlayerID]->leftStick_X() > 0.1f /*|| m_pDinput->isKeyDown(DIK_D)*/)
		{
			_rTranslate += D3DXVECTOR2(1, 0);
			_bMoveHandled = true;
		}

		if (_bMoveHandled)
		{
			m_inputTimersMove[_iPlayerID].second = m_inputTimersMove[_iPlayerID].first;
			D3DXVec2Normalize(&_rTranslate, &_rTranslate);
			return true;
		}
	}
	return false;
}

/**
*	Update gamepad
*	@author Serge Radinovich
*/
void InputHandler::update()
{
	for (UINT i = 0; i < m_pGamePads.size(); ++i)
	{
		m_pGamePads[i]->refreshState();
		m_pGamePads[i]->poll();
	}
		
	m_pDinput->pollKeyboard();
	m_pDinput->pollMouse();
}


bool InputHandler::handleGeneralButton(UINT _iPlayerID, E_GAMEPAD_BUTTON _eButtonRequested, float _fDeltaTime)
{
	
	static float s_fLastHandle = 0.0f;
	static float s_fTimeElapsed = 0.0f;

	s_fTimeElapsed += _fDeltaTime;

	if (s_fTimeElapsed > 0.5f && m_pGamePads[_iPlayerID]->getButtonDown(_eButtonRequested))
	{
		s_fTimeElapsed = 0.0f;
		return true;
	}
	return false;
}

bool InputHandler::handleGeneralKB(char _cButton, float _fDeltaTime)
{
	static float s_fLastHandle = 0.0f;
	static float s_fTimeElapsed = 0.0f;

	s_fTimeElapsed += _fDeltaTime;

	if (s_fTimeElapsed > 0.3f && m_pDinput->isKeyDown(_cButton))
	{
		s_fTimeElapsed = 0.0f;
		return true;
	}
	return false;
}

bool InputHandler::handleMouseClick(int _iMouseButtonIndex, float _fDeltaTime, pair<float, float>& _rMousePos)
{
	static float s_fLastHandle = 0.0f;
	static float s_fTimeElapsed = 0.0f;

	s_fTimeElapsed += _fDeltaTime;

	if (s_fTimeElapsed > 0.5f && m_pDinput->isMouseButtonDown(_iMouseButtonIndex))
	{
		_rMousePos.first = mouseScreenCoordX();
		_rMousePos.second = mouseScreenCoordY();
		s_fTimeElapsed = 0.0f;
		return true;
	}
	return false;
}

float InputHandler::mouseScreenCoordX()
{

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_pDinput->getWindowHandle(), &p);
	if (p.x > SCREEN_WIDTH_WINDOWED || p.x < 0 || p.y > SCREEN_HEIGHT_WINDOWED || p.y < 0) //TODO: Remove hard-coded screen size numebrs
		return 0.0f;

	return ((float)p.x / SCREEN_WIDTH_WINDOWED) * 2 - 1;

}

float InputHandler::mouseScreenCoordY()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_pDinput->getWindowHandle(), &p);

	if (p.x > SCREEN_WIDTH_WINDOWED || p.x < 0 || p.y > SCREEN_HEIGHT_WINDOWED || p.y < 0)  //TODO: Remove hard-coded screen size numebrs
		return 0.0f;
	return ((float)p.y / -SCREEN_HEIGHT_WINDOWED) * 2 + 1;
}