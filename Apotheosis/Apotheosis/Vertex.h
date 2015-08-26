// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Vertex.h
// Description :  Vertex header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#pragma once
#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Util.h"



	//**********************************************************************
	struct VertexPNT
	{
		VertexPNT()
		{
		}

		VertexPNT(const D3DXVECTOR3& _rkPos, const D3DXVECTOR3& _rkNormal, const D3DXVECTOR2 _rkUV)
		{
			pos = _rkPos;
			normal = _rkNormal;
			D3DXVec3Normalize(&normal, &normal);
			texC = _rkUV;

		}

		VertexPNT(float x, float y, float z,
			float nx, float ny, float nz,
			float u, float v)
			: pos(x, y, z)
			, normal(nx, ny, nz)
			, texC(u, v)
		{}

		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 texC;

	};

	const D3D10_INPUT_ELEMENT_DESC g_kInputLayoutVertexPNT[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	//////////////////////////////////////////////////////////////////////////


#endif //__VERTEX_H__