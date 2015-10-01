// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Util.h
// Description :  Util header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#pragma once

#ifndef __UTIL_H__
#define __UTIL_H__

#include <d3d10.h>
#include <d3dx10.h>
#include <dxerr.h>
#include <cassert>
#include <vector>
#include <array>
#include <string>
#include "Vertex.h"
#include "Box2D.h"
using namespace std;



// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D Objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
				{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
						{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
						}                                                      \
				}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 



#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#pragma warning(disable:4351) //Default initialization of char data[256]


enum E_SCENE : UINT { S_MAINMENU, S_GAMEPLAY, S_MAX };



const int SCREEN_BOUND = 40;


	// Randomness
	float randomFloat(float a, float b);
	void randomVec(D3DXVECTOR3* pOut);

	//Equality
	bool equal(float _fA, float _fB, float _fAccuracy = 0.001f);

	std::string resourceDirectory();

	b2Vec2 DXTob2(const D3DXVECTOR2 _rkVecDX);
	D3DXVECTOR2 b2ToDX(const b2Vec2 _rkVecB2);



	const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
	const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
	const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
	const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
	const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

	const D3DXCOLOR BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
	const D3DXCOLOR LIGHT_GREY(0.35f, 0.35f, 0.35f, 1.0f);
	const D3DXCOLOR LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
	const D3DXCOLOR DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
	const D3DXCOLOR DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);



//Filled out in winmain by g_devEnum to be passed to DX9Renderer for init
struct D3DInitializer// : public IRendererInitializer
{
	bool	 bIsWindowed{ true };
	unsigned iHeight{ 600 };
	unsigned iWidth{ 800 };
	bool	 bVSYNC{ false };

};



//DX9Renderer & winmain
const int SCREEN_WIDTH_MAX = GetSystemMetrics(SM_CXSCREEN);
const int SCREEN_HEIGHT_MAX = GetSystemMetrics(SM_CYSCREEN);
const int SCREEN_WIDTH_WINDOWED = 800;
const int SCREEN_HEIGHT_WINDOWED = 600;


#endif //__UTIL_H__