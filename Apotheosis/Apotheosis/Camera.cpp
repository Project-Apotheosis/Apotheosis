// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Camera.cpp
// Description : Camera cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "Camera.h"
#include "Util.h"

	/**
	*	Constructor
	*	@author Serge Radinovich
	*/
	Camera::Camera()
		: m_lookPos(0.0f, 0.0f, 0.0f)
		, m_fAspectRatio(0.0f)
	{
	}

	/**
	*	Destructor
	*	@author Serge Radinovich
	*/
	Camera::~Camera()
	{
	}

	/**
	*	Initialize the camera with an aspect ratio
	*	@author Serge Radinovich
	*	@param  _kfAspectRatio		Aspect ratio
	*/
	void Camera::init(const float _kfAspectRatio)
	{
		m_fAspectRatio = _kfAspectRatio;
		updateProjectionMatrix();
	}

	/**
	*	Set the position of the camera and update its rendering matrices accordingly
	*	@author Serge Radinovich
	*	@param  _pPos		New position of the camera
	*/
	void Camera::setPos(const D3DXVECTOR3* _pPos)
	{
		m_transform.translation = *_pPos;
		updateViewMatrix();
	}

	/**
	*	Set the position at which the camera is looking and update its rendering matrices accordingly
	*	@author Serge Radinovich
	*	@param  _pTarget		New target of the camera
	*/
	void Camera::lookAt(const D3DXVECTOR3*  _pTarget)
	{
		m_lookPos = *_pTarget;

		D3DXVECTOR3 _look = m_lookPos - m_transform.translation;
		D3DXVec3Normalize(&_look, &_look);

		Transform::setForward(&m_transform, &_look);

		updateViewMatrix();
	}

	/**
	*	Use the transform of the camera to create the view matrix used for rendering (by taking its inverse)
	*   This is called within camera functions every time its transform is changed
	*	@author Serge Radinovich
	*/
	void Camera::updateViewMatrix()
	{
		D3DXMATRIX _worldMtx = *m_transform.matrix();

		float _fDeterminant = D3DXMatrixDeterminant(&_worldMtx);
		D3DXMatrixInverse(&_worldMtx, &_fDeterminant, &_worldMtx);

		m_viewMtx = _worldMtx;

	}
	
	/**
	*	Use the aspect ratio and the clipping planes of the camera to create the projection matrix used for rendering (by taking its inverse)
	*   This is called within camera functions every time the aspect ratio or clipping planes change
	*	@author Serge Radinovich
	*/
	void Camera::updateProjectionMatrix()
	{
		//Specify the field of view, aspect ration and near and far clipping planes.
		//D3DXMatrixPerspectiveFovLH(&m_projMtx, (float)(D3DX_PI / 4.0), m_fAspectRatio, m_kfNearClip, m_kfFarClip); 
		D3DXMatrixOrthoLH(&m_projMtx, SCREEN_WIDTH_MAX / 13, SCREEN_HEIGHT_MAX / 13, m_kfNearClip, m_kfFarClip);
	}

	/**
	*	Projection matrix getter
	*	@author Serge Radinovich
	*	@return	 const D3DXMATRIX*	Pointer to camera's projection matrix
	*/
	const D3DXMATRIX* Camera::projectionMatrix() const
	{
		return &m_projMtx;
	}
	
	/**
	*	View matrix getter
	*	@author Serge Radinovich
	*	@return	 const D3DXMATRIX*	Pointer to camera's view matrix
	*/
	const D3DXMATRIX* Camera::viewMatrix() const
	{
		return &m_viewMtx;
	}

	/**
	*	Translate the camera in world-space and update rendering matrices accordingly
	*	@author Serge Radinovich
	*	@param	_translationVec		Pointer to vector that is used to translate the camera
	*/
	void Camera::translateAbs(const D3DXVECTOR3* _translationVec)
	{
		m_transform.translation += *_translationVec;
		updateViewMatrix();
	}

	/**
	*	Translate the camera in object-space and update rendering matrices accordingly
	*	@author Serge Radinovich
	*	@param	_translationVec		Pointer to vector that is used to translate the camera
	*/
	void Camera::translateRel(const D3DXVECTOR3* _translationVec)
	{
		Transform::translateRel(&m_transform, _translationVec);
		updateViewMatrix();
	}

	/**
	*	Rotate the camera in object-space and update rendering matrices accordingly
	*	@param	_fAngle		Radians used to rotate the camera
	*	@author Serge Radinovich
	*/
	void Camera::rotateAroundUpRel(float _fAngle)
	{
		Transform::rotateAroundAxis(&m_transform, &m_transform.up(), _fAngle);
		updateViewMatrix();
	}

	/**
	*	Rotate the camera in object-space and update rendering matrices accordingly
	*	@author Serge Radinovich
	*	@param	_fAngle		Radians used to rotate the camera
	*/
	void Camera::rotateAroundRightRel(float _fAngle)
	{
		Transform::rotateAroundAxis(&m_transform, &m_transform.right(), _fAngle);
		updateViewMatrix();
	}

	/**
	*	Rotate the camera in world-space and update rendering matrices accordingly
	*	@author Serge Radinovich
	*	@param	_fAngle		Radians used to rotate the camera
	*/
	void Camera::rotateAroundUp(float _fAngle)
	{
		Transform::rotateAroundUp(&m_transform, _fAngle);
		updateViewMatrix();
	}

	/**
	*	Rotate the camera in world-space and update rendering matrices accordingly
	*	@author Serge Radinovich
	*	@param	_fAngle		Radians used to rotate the camera
	*/
	void Camera::rotateAroundRight(float _fAngle)
	{
		Transform::rotateAroundRight(&m_transform, _fAngle);
		updateViewMatrix();
	}

	/**
	*	Rotate the camera around a point and an axis
	*	@author Serge Radinovich
	*	@param	point		Point upon which to rotate around
	*	@param	axis		Axis upon which to rotate around
	*	@param	_fAngle		Radians used to rotate the camera
	*/
	void Camera::rotateAroundPoint(const D3DXVECTOR3* point, const D3DXVECTOR3* axis, float _fAngle)
	{
		//m_camObj.rotateAroundPoint(point, axis, _fAngle);
		Transform::rotateAroundPoint(&m_transform, point, axis, _fAngle);
		updateViewMatrix();
	}

	/**
	*	Update rendering matrices when device reset
	*	@author Serge Radinovich
	*/
	void Camera::onResetDevice()
	{
		updateProjectionMatrix();
		updateViewMatrix();
	}

	/**
	*	Set forward vector of camera and update rendering matrices accordingly
	*	@author Serge Radinovich
	*	@param	_pkForward		New forward
	*/
	void Camera::setForwardVec(const D3DXVECTOR3* _pkForward)
	{
		Transform::setForward(&m_transform, _pkForward);
		updateViewMatrix();
	}

	/**
	*	Transform matrix getter
	*	@author Serge Radinovich
	*	@return	const D3DXMATRIX*	Transform matrix
	*/
	const D3DXMATRIX* Camera::getObjTransform()
	{
		//return m_camObj.getObjTransform();
		return m_transform.matrix();
	}

	/**
	*	Transform matrix setter
	*	@author Serge Radinovich
	*	@param	const D3DXMATRIX*	New transform matrix
	*/
	void Camera::setTransform(const D3DXMATRIX* _pkTransform)
	{
		//m_camObj.setTransform(_pkTransform);
		Transform::setViaMatrix(&m_transform, _pkTransform);
		updateViewMatrix();
	}

	/**
	*	Camera position getter
	*	@author Serge Radinovich
	*	@return	D3DXVECTOR3	 Camera position
	*/
	D3DXVECTOR3 Camera::getPos()
	{
		//return m_camObj.getPos();
		return m_transform.translation;
	}

	/**
	*	Forward vector getter
	*	@author Serge Radinovich
	*	@return	D3DXVECTOR3	 Forward vector 
	*/
	D3DXVECTOR3 Camera::getForward()
	{
		return m_transform.forward();
	}

	


