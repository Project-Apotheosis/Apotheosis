// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Util.h
// Description :  Util cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "Util.h"


b2Vec2 DXTob2(const D3DXVECTOR2 _rkVecDX)
{
	b2Vec2 _returnVec;
	_returnVec.x = _rkVecDX.x;
	_returnVec.y = _rkVecDX.y;
	return _returnVec;
}
D3DXVECTOR2 b2ToDX(const b2Vec2 _rkVecB2)
{
	D3DXVECTOR2 _returnVec;
	_returnVec.x = _rkVecB2.x;
	_returnVec.y = _rkVecB2.y;
	return _returnVec;
}

bool equal(float _fA, float _fB, float _fAccuracy)
{
	return fabsf(_fA - _fB) < _fAccuracy;
}

// Returns a random float in [a, b].
float randomFloat(float a, float b)
{
	if (a >= b) // bad input
		return a;

	// Get random float in [0, 1] interval.
	float f = (rand() % 10001) * 0.0001f;

	return (f*(b - a)) + a;
}

// Returns a random unit vector on the unit sphere.
void randomVec(D3DXVECTOR3* _pOut)
{
	_pOut->x = randomFloat(-1.0f, 1.0f);
	_pOut->y = randomFloat(-1.0f, 1.0f);
	_pOut->z = randomFloat(-1.0f, 1.0f);

	// Project onto unit sphere.
	D3DXVec3Normalize(_pOut, _pOut);
}


std::string resourceDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string _returnString = std::string(buffer).substr(0, pos);

	pos = _returnString.find_last_of("\\/");
	_returnString = std::string(buffer).substr(0, pos);
	_returnString.append("\\Resources\\");

	return _returnString;
}