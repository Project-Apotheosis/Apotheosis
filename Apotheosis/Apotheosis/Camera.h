// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Camera.h
// Description :  Camera header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com


#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Util.h"
#include "Transform.h"


	class DX10Renderer;
	//Interface to renderer's functionality for creating homogenous clip-space data for device
	//All transformations on Camera object update its transformation matrix
	class Camera
	{

	public: //Construction

		friend class DX10Renderer;

		Camera();
		~Camera();

		void init(const float _kfAspectRatio);

	private: //Data

		D3DXMATRIX m_viewMtx;
		D3DXMATRIX m_projMtx;
		D3DXMATRIX m_viewProjMtx;

		Transform m_transform;
		D3DXVECTOR3 m_lookPos;

		const float m_kfNearClip{ 1.0f };
		const float m_kfFarClip{ 10000.0f };

		float m_fAspectRatio;


	private: //Internals

		//Called in onResetDevice() and init() to set up projection
		void updateProjectionMatrix();
		//Called every time camera object is transformed
		void updateViewMatrix();
	

	public: //Interface

		//bool isVisible(const AABB* box) const;


		//Translate / Rotate
		//Translate by [in] world space vector
		void translateAbs(const D3DXVECTOR3* translationVec);
		//Translate by [in] object space vector
		void translateRel(const D3DXVECTOR3* translationVec);
		//Rotate relative to current up vector
		void rotateAroundUp(float);
		//Rotate relative to current right vector
		void rotateAroundRight(float);
		void rotateAroundUpRel(float);
		void rotateAroundRightRel(float);
		//Rotate around [in] vector, centered at [in] position
		void rotateAroundPoint(const D3DXVECTOR3* point, const D3DXVECTOR3* axis, float);

		//Set transforms
		void setForwardVec(const D3DXVECTOR3*);
		void setTransform(const D3DXMATRIX*);
		void lookAt(const D3DXVECTOR3*);
		void setPos(const D3DXVECTOR3*);

		//Get transforms
		D3DXVECTOR3 getPos();
		D3DXVECTOR3 getForward();
		const D3DXMATRIX* getObjTransform();
		const D3DXMATRIX* projectionMatrix() const;
		const D3DXMATRIX* viewMatrix() const;

		void onResetDevice();

	};

#endif //__CAMERA_H__