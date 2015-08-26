// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Transform.h
// Description :  Transform header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#pragma once

#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Util.h"



	struct Transform
	{
	public: //Construction
		Transform();
		~Transform();

	public: //Data
		//SQT
		D3DXQUATERNION		rotation;
		float				scale;
		D3DXVECTOR3			translation;

	private: //Render matrix

		D3DXMATRIX renderMatrix; //Held in stack to speed up access to current transform (don't have to create matrix every time requested)

	public: //Interface
		//Getters
		//Create and return a matrix for rendering purposes
		const D3DXMATRIX*  matrix();
		D3DXVECTOR3 right() const;
		D3DXVECTOR3 up() const;
		D3DXVECTOR3 forward() const;

	public: //Statics

		//Setting
		static void setViaMatrix(Transform* out, const D3DXMATRIX* newTransform);

		static void setForward(Transform* out, const D3DXVECTOR3*);
		static void setUp(Transform* out, const D3DXVECTOR3*);
		static void setRight(Transform* out, const D3DXVECTOR3*);
		static void setTranslation(Transform* out, const D3DXVECTOR3*);
		static void setTranslation(Transform* out, float, float, float);
		static void vecXquat(D3DXVECTOR3* out, const D3DXQUATERNION*, const D3DXVECTOR3*);
		static void vecXquat(D3DXQUATERNION* out, const D3DXQUATERNION*, const D3DXVECTOR3*);


		//Translating
		static D3DXVECTOR3 getRelativeVector(Transform* out, const D3DXVECTOR3*);
		//Absolute translation is done by adding a vector to translation	
		//Translate by [in] object space vector
		static void translateRel(Transform* out, const D3DXVECTOR3*);
		//Translate by [in] object space vector
		static void translateRel(Transform* out, float, float, float);

		//Rotate relative to current up vector
		static void rotateAroundUp(Transform* out, float radians);
		//Rotate relative to current right vector
		static void rotateAroundRight(Transform* out, float radians);
		static void rotateAroundAxis(Transform* out, const D3DXVECTOR3*, float radians);
		static void rotateAroundPoint(Transform* out, const D3DXVECTOR3* point, const D3DXVECTOR3* axis, float radians);

		//Steer transform toward new orientation based on newForward
		static void rotateToward(Transform* out, const D3DXVECTOR3* newForward, float interpFactor);

	};

#endif //__TRANSFORM_H__