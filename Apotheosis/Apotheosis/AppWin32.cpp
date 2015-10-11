// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : AppWin32.cpp
// Description :  AppWin32 cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "AppWin32.h"
#include "commctrl.h"



	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); //Declaration of friend function which calls the app's window procedure or default window proc

	AppWin32* AppWin32::s_pInstance = nullptr;				 //Singleton

	/**
	*	Constructor
	*	@author Serge Radinovich
	*/
	AppWin32::AppWin32()
	{
		ZeroMemory(&m_msg, sizeof(m_msg));
		ZeroMemory(&m_hInst, sizeof(m_hInst));
		ZeroMemory(&m_hWnd, sizeof(m_hWnd));
	}

	/**
	*	Destructor
	*	@author Serge Radinovich
	*/
	AppWin32::~AppWin32()
	{
	}

	/**
	*	Initialize the application window and GameRoot
	*	@author Serge Radinovich
	*	@param _hInst	Handle to instance relating to this application
	*	@param _pIRoot	Pointer to GameRoot through base class which contains screen size information
	*/
	bool AppWin32::init(HINSTANCE _hInst, IRoot* _pIRoot)
	{
		m_pRoot = _pIRoot;

		srand(static_cast<UINT>(time(0)));

		ZeroMemory(&m_hWnd, sizeof(m_hWnd));

		//Create window
		if (!initWindow(_hInst, &m_pRoot->m_initializer))
		{
			MessageBox(NULL, L"Unable to create window", L"ERROR", MB_OK);
			return false;
		}

		//Initialize D3D		
		if (!m_pRoot->init(m_hWnd, m_hInst))
		{
			MessageBox(NULL, L"Unable to create D3D Device", L"ERROR", MB_OK);
			return false;
		}

		return true;

	}

	/**
	*	Initialize the application window
	*	@author Serge Radinovich
	*	@param _hInst			Handle to instance relating to this application
	*	@param _pInitializer	Screen and D3D data holder for initializing window
	*/
	bool AppWin32::initWindow(HINSTANCE _hInst, const D3DInitializer* _pInitializer)
	{
		WNDCLASS _wc;
		_wc.style = CS_HREDRAW | CS_VREDRAW;
		_wc.lpfnWndProc = (WNDPROC)WindowProc;
		_wc.cbClsExtra = 0;
		_wc.cbWndExtra = 0;
		_wc.hInstance = _hInst;
		_wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		_wc.hCursor = LoadCursor(0, IDC_ARROW);
		_wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		_wc.lpszMenuName = 0;
		_wc.lpszClassName = L"D3DWndClassName";

		if (!RegisterClass(&_wc))
		{
			MessageBox(0, L"RegisterClass FAILED", 0, 0);
			PostQuitMessage(0);
		}

		m_hWnd = CreateWindowW(L"D3DWndClassName", L"Apotheosis", WS_BORDER, 0, 0, _pInitializer->iWidth, _pInitializer->iHeight, 0, 0, _hInst, 0);


		if (!m_hWnd)
			return false;

		// Set the global instance handle
		m_hInst = _hInst;

		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);

		return true;
	}



	/**
	*	Shut down singleton
	*	@author Serge Radinovich
	*/
	void AppWin32::shutDown()
	{
		m_pRoot->shutDown();

		SAFE_DELETE(s_pInstance);
	}



	/**
	*	Friend function which is the application's way of accessing AppWin32 message pump or default pump
	*	@author Serge Radinovich
	*	@param _hWnd	Handle to window that this application is using
	*	@param _msg		Current message processed by message pump
	*	@param _wParam	Message data
	*	@param _lParam	Message data
	*/
	LRESULT CALLBACK WindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
	{
		AppWin32* _pApp = AppWin32::getInstancePtr();
		if (_pApp != nullptr)
		{
			return _pApp->mWindowProc(_hWnd, _msg, _wParam, _lParam);
		}
		else
		{
			return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
		}
	}

	/**
	*	Update loop for the application, called from winmain
	*	@author Serge Radinovich
	*/
	void AppWin32::run()
	{

		// Main message loop:
		// Enter the message loop
		while (m_msg.message != WM_QUIT)
		{
			//Check for messages
			if (PeekMessage(&m_msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
			//This is called when no messages are pending
			else
			{
				if (!m_pRoot->executeOneFrame(m_bPaused)) 
					PostQuitMessage(0);
			}
		}

	}

	/**
	*	Member function controlling message pump, called by CALLBACK friend function defined above
	*	@author Serge Radinovich
	*	@param _hWnd	Handle to window that this application is using
	*	@param _msg		Current message processed by message pump
	*	@param _wParam	Message data
	*	@param _lParam	Message data
	*/
	LRESULT CALLBACK AppWin32::mWindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
	{

		switch (_msg)
		{

		case WM_ACTIVATE: //When application is activated or deactivated
		{
			if (LOWORD(_wParam) == WA_INACTIVE)
			{
				m_pRoot->m_timer.stop();
				m_bPaused = true;
			}
			else
			{
				m_pRoot->m_timer.start();
				m_bPaused = false;
			}
			return 0;
		}


		case WM_SIZE: // WM_SIZE is sent when the user resizes the window OR when alt+tab is pressed
			// Save the new client area dimensions.
			m_pRoot->m_initializer.iWidth = LOWORD(_lParam); //TODO: Figure out why these values are never rounded even for full-screen (problems for refresh rate test in DX10Renderer
			m_pRoot->m_initializer.iHeight = HIWORD(_lParam);

			if (m_pRoot->m_pRenderer) //Only if root has initialized renderer
			{

				if (_wParam == SIZE_MINIMIZED)
				{
					m_bPaused = true;
					m_bMinimized = true;
					m_bMaximized = false;
				}
				else if (_wParam == SIZE_MAXIMIZED)
				{
					m_bPaused = false;
					m_bMinimized = false;
					m_bMaximized = true;
					m_pRoot->m_pRenderer->onResize();
				}
				else if (_wParam == SIZE_RESTORED)
				{

					// Restoring from minimized state?
					if (m_bMinimized)
					{
						m_bPaused = false;
						m_bMinimized = false;
						m_pRoot->m_pRenderer->onResize();
					}

					// Restoring from maximized state?
					else if (m_bMaximized)
					{
						m_bPaused = false;
						m_bMaximized = false;
						m_pRoot->m_pRenderer->onResize();
					}
					else if (m_bResizing)
					{
						// If user is dragging the resize bars, we do not resize 
						// the buffers here because as the user continuously 
						// drags the resize bars, a stream of WM_SIZE messages are
						// sent to the window, and it would be pointless (and slow)
						// to resize for each WM_SIZE message received from dragging
						// the resize bars.  So instead, we reset after the user is 
						// done resizing the window and releases the resize bars, which 
						// sends a WM_EXITSIZEMOVE message.
					}
					else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
					{
						m_pRoot->m_pRenderer->onResize();
					}
				}
			}
			return 0;


		case WM_ENTERSIZEMOVE: //When user grabs the resize bars
		{
			m_pRoot->m_timer.stop();
			m_bPaused = true;
			m_bResizing = true;

			return 0;
		}

		case WM_EXITSIZEMOVE: //When user releases the resize bars. Reset everything based on new window dimensions
		{
			m_pRoot->m_timer.start();
			m_bPaused = false;
			m_bResizing = false;
			m_pRoot->m_pRenderer->onResize(); //Let renderer resize accordigly
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			return 0;
		}

		case WM_KEYDOWN:

			if (_wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				break;
			}


			return 0;

		case WM_DESTROY:

			PostQuitMessage(0);
			return 0;


		case WM_MENUCHAR: //When a menu is active and the user presses a key that does not correspond to any mnemonic or accelerator key
			return MAKELRESULT(0, MNC_CLOSE);

		case WM_GETMINMAXINFO: //Catch this message to prevent the window from becoming too small
			((MINMAXINFO*)_lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)_lParam)->ptMinTrackSize.y = 200;
		}
		return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
	}



