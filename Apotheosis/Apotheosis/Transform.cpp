// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : Transform.cpp
// Description :  Transform cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include "Transform.h"


	/**
	* Constructor
	* @author	Serge Radinovich
	*/
	Transform::Transform()
		: scale(1.0f)
		, translation(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		D3DXQuaternionIdentity(&rotation); //MF: D3DX Usage
		D3DXMatrixIdentity(&renderMatrix); //MF: D3DX Usage
	}

	/**
	* Destuctor
	* @author	Serge Radinovich
	*/
	Transform::~Transform()
	{
	}

	/**
	* Return this transform in matrix form
	* @author	Serge Radinovich
	* @return	D3DXMATRIX*			Matrix returned
	*/
	const D3DXMATRIX*  Transform::matrix()
	{
		//Rotate
		D3DXMatrixRotationQuaternion(&renderMatrix, &rotation); //MF: D3DX Usage
		//Translate
		renderMatrix(3, 0) = translation.x;
		renderMatrix(3, 1) = translation.y;
		renderMatrix(3, 2) = translation.z;
		//Scale
		D3DXMATRIX _scaleMtx;
		D3DXMatrixScaling(&_scaleMtx, scale, scale, scale);
		D3DXMatrixMultiply(&renderMatrix, &_scaleMtx, &renderMatrix); //MF: D3DX Usage

		return &renderMatrix;
	}

	/**
	* Return current right vector in world space
	* @author	Serge Radinovich
	* @return	D3DXMATRIX			Right vector returned
	*/
	D3DXVECTOR3 Transform::right() const
	{
		D3DXVECTOR3 _right;
		vecXquat(&_right, &rotation, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		return _right;
	}

	/**
	* Return current forward vector in world space
	* @author	Serge Radinovich
	* @return	D3DXMATRIX			Forward vector returned
	*/
	D3DXVECTOR3 Transform::forward() const
	{
		D3DXVECTOR3 _fwd;
		vecXquat(&_fwd, &rotation, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
		return _fwd;
	}

	/**
	* Return current up vector in world space
	* @author	Serge Radinovich
	* @return	D3DXMATRIX			Up vector returned
	*/
	D3DXVECTOR3 Transform::up() const
	{
		D3DXVECTOR3 _up;
		vecXquat(&_up, &rotation, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		return _up;
	}


	/**
	* Multiply vector by quaternion
	* @author	Serge Radinovich
	* @param	_pOutV			Return vector product
	* @param	_pInQ			Quaternion to multiply vector with
	* @param	_pInV			Vector to multiply quaternion with
	*/
	void Transform::vecXquat(D3DXVECTOR3* _pOutV, const D3DXQUATERNION* _pInQ, const D3DXVECTOR3* _pInV)
	{
		D3DXQUATERNION q1, q2;
		D3DXQuaternionConjugate(&q1, _pInQ);
		q2 = q1 * D3DXQUATERNION(_pInV->x, _pInV->y, _pInV->z, 1.0f) * *_pInQ;
		*_pOutV = D3DXVECTOR3(q2.x, q2.y, q2.z);
	}

	/**
	* Multiply quaternion by vector
	* @author	Serge Radinovich
	* @param	_pOutQ			Return quaternion product
	* @param	_pInQ			Quaternion to multiply vector with
	* @param	_pInV			Vector to multiply quaternion with
	*/
	void Transform::vecXquat(D3DXQUATERNION* _pOutQ, const D3DXQUATERNION* _pInQ, const D3DXVECTOR3* _pInV)
	{
		D3DXQUATERNION q1, q2;
		D3DXQuaternionConjugate(&q1, _pInQ);//MF: D3DX Usage
		q2 = q1 * D3DXQUATERNION(_pInV->x, _pInV->y, _pInV->z, 1.0f) * *_pInQ;

		*_pOutQ = q2;
	}

	/**
	* Set transform with a matrix incoming
	* @author	Serge Radinovich
	* @param	_pOut					New Transform result
	* @param	_pkNewTransformMtx		Transformation matrix to create this transform from
	*/
	void Transform::setViaMatrix(Transform* _pOut, const D3DXMATRIX* _pkNewTransformMtx)
	{
		D3DXQuaternionRotationMatrix(&_pOut->rotation, _pkNewTransformMtx);//MF: D3DX Usage
		_pOut->translation.x = (*_pkNewTransformMtx)(3, 0);
		_pOut->translation.y = (*_pkNewTransformMtx)(3, 1);
		_pOut->translation.z = (*_pkNewTransformMtx)(3, 2);
	}



	/**
	* Set transform's translation
	* @author	Serge Radinovich
	* @param	_pOut				New Transform result
	* @param	_pkNewPos			New translation
	*/
	void Transform::setTranslation(Transform* _pOut, const D3DXVECTOR3* _pkNewPos)
	{
		_pOut->translation = *_pkNewPos;
	}

	/**
	* Set transform's translation
	* @author	Serge Radinovich
	* @param	_pOut		New Transform result
	* @param	_fX			New translation
	* @param	_fY			New translation
	* @param	_fZ			New translation
	*/
	void Transform::setTranslation(Transform* _pOut, float _fX, float _fY, float _fZ)
	{
		_pOut->translation.x = _fX;
		_pOut->translation.y = _fY;
		_pOut->translation.z = _fZ;
	}

	/**
	* Set transform by using a forward vector
	* @author	Serge Radinovich
	* @param	_pOut				New Transform result
	* @param	_pNewForward		New forward vector for transform
	*/
	void Transform::setForward(Transform* _pOut, const D3DXVECTOR3* _pNewForward)
	{
		//Turn into matrix then into quaternion
		D3DXVECTOR3 _fwd = *_pNewForward;
		D3DXVECTOR3 _right;
		D3DXVECTOR3 _up;
		D3DXVec3Normalize(&_fwd, &_fwd);
		//Find new right
		D3DXVec3Cross(&_right, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &_fwd);
		D3DXVec3Normalize(&_right, &_right);
		//Find new up
		D3DXVec3Cross(&_up, &_fwd, &_right);
		D3DXVec3Normalize(&_up, &_up);

		D3DXMATRIX _fwdMtx;
		_fwdMtx(0, 0) = _right.x;	_fwdMtx(0, 1) = _right.y;	_fwdMtx(0, 2) = _right.z;
		_fwdMtx(1, 0) = _up.x;		_fwdMtx(1, 1) = _up.y;		_fwdMtx(1, 2) = _up.z;
		_fwdMtx(2, 0) = _fwd.x;		_fwdMtx(2, 1) = _fwd.y;		_fwdMtx(2, 2) = _fwd.z;
		D3DXQuaternionRotationMatrix(&_pOut->rotation, &_fwdMtx);

	}

	/**
	* Set transform by using a right vector
	* @author	Serge Radinovich
	* @param	_pOut				New Transform result
	* @param	_pNewRight			New right vector for transform
	*/
	void Transform::setRight(Transform* _pOut, const D3DXVECTOR3* _pNewRight)
	{
		//Turn into matrix then into quaternion
		D3DXVECTOR3 _right = *_pNewRight;
		D3DXVECTOR3 _fwd;
		D3DXVECTOR3 _up;
		D3DXVec3Normalize(&_right, &_right);

		//Find new fwd
		D3DXVec3Cross(&_fwd, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &_right);
		D3DXVec3Normalize(&_fwd, &_fwd);

		//Find new up
		D3DXVec3Cross(&_up, &_right, &_fwd);
		D3DXVec3Normalize(&_up, &_up);

		D3DXMATRIX _fwdMtx;
		_fwdMtx(0, 0) = _right.x;	_fwdMtx(0, 1) = _right.y;	_fwdMtx(0, 2) = _right.z;
		_fwdMtx(1, 0) = _up.x;		_fwdMtx(1, 1) = _up.y;		_fwdMtx(1, 2) = _up.z;
		_fwdMtx(2, 0) = _fwd.x;		_fwdMtx(2, 1) = _fwd.y;		_fwdMtx(2, 2) = _fwd.z;
		D3DXQuaternionRotationMatrix(&_pOut->rotation, &_fwdMtx);//MF: D3DX Usage
	}

	/**
	* Set transform by using a up vector
	* @author	Serge Radinovich
	* @param	_pOut				New Transform result
	* @param	_pkNewUp			New up vector for transform
	*/
	void Transform::setUp(Transform* _pOut, const D3DXVECTOR3* _pkNewUp)
	{
		//assert(false && "Transform::setUp() not yet implemented");
		D3DXVECTOR3 _up;
		D3DXVECTOR3 _right;
		D3DXVECTOR3 _fwd;

		D3DXVec3Normalize(&_up, _pkNewUp);

		//Find new right
		D3DXVec3Cross(&_right, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &_up);
		D3DXVec3Normalize(&_right, &_right);


		//Find new forward
		D3DXVec3Cross(&_fwd, &_up, &_right);
		D3DXVec3Normalize(&_fwd, &_fwd);

		D3DXMATRIX _fwdMtx;
		_fwdMtx(0, 0) = _right.x;	_fwdMtx(0, 1) = _right.y;	_fwdMtx(0, 2) = _right.z;
		_fwdMtx(1, 0) = _up.x;		_fwdMtx(1, 1) = _up.y;		_fwdMtx(1, 2) = _up.z;
		_fwdMtx(2, 0) = _fwd.x;		_fwdMtx(2, 1) = _fwd.y;		_fwdMtx(2, 2) = _fwd.z;
		D3DXQuaternionRotationMatrix(&_pOut->rotation, &_fwdMtx);//MF: D3DX Usage
	}

	/**
	*
	* Rotate a vector to be in local space of an object's transform
	* @author	Serge Radinovich
	* @param	_pOut		 Transform from which the relative vector is being calculated.
	* @return				 Vector rotated to be relative to object's axis
	*/
	D3DXVECTOR3 Transform::getRelativeVector(Transform* _pOut, const D3DXVECTOR3* _pkTranslation)
	{
		D3DXVECTOR3 _rotatedTranslation;
		vecXquat(&_rotatedTranslation, &_pOut->rotation, _pkTranslation);

		return _rotatedTranslation;
	}

	/**
	* Translate transform in model space. Translation must be rotated into model space here
	* @author	Serge Radinovich
	* @param	_pOut				New Transform result
	* @param	_pkTranslation		Translation to be rotated into model space and added to transform translation
	*/
	void Transform::translateRel(Transform* _pOut, const D3DXVECTOR3* _pkTranslation)
	{
		D3DXVECTOR3 _rotatedTranslation;
		vecXquat(&_rotatedTranslation, &_pOut->rotation, _pkTranslation);

		_pOut->translation += _rotatedTranslation;
	}

	/**
	* Translate transform in model space. Translation must be rotated into model space here
	* @author	Serge Radinovich
	* @param	_pOut		New Transform result
	* @param	_fX			Translation
	* @param	_fY			Translation
	* @param	_fZ			Translation
	*/
	void Transform::translateRel(Transform* _pOut, float _fX, float _fY, float _fZ)
	{
		translateRel(_pOut, &D3DXVECTOR3(_fX, _fY, _fZ));
	}

	/**
	* Rotate transform around world-space up
	* @author	Serge Radinovich
	* @param	_pOut			New Transform result
	* @param	_fRadians		Radians to rotate by
	*/
	void Transform::rotateAroundUp(Transform* _pOut, float _fRadians)
	{
		D3DXQUATERNION _rotQ;
		D3DXQuaternionRotationAxis(&_rotQ, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), _fRadians);

		_pOut->rotation *= _rotQ; //Todo: check if needs other way around
	}

	/**
	* Rotate transform around wordl-space right
	* @author	Serge Radinovich
	* @param	_pOut			New Transform result
	* @param	_fRadians		Radians to rotate by
	*/
	void Transform::rotateAroundRight(Transform* _pOut, float _fRadians)
	{
		D3DXQUATERNION _rotQ;
		D3DXQuaternionRotationAxis(&_rotQ, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), _fRadians);

		_pOut->rotation *= _rotQ; //Todo: check if needs other way around
	}

	/**
	* Rotate transform around provided axis
	* @author	Serge Radinovich
	* @param	_pOut			New Transform result
	* @param	_pkAxis			Axis around which to rotate transform
	* @param	_fRadians		Radians to rotate by
	*/
	void Transform::rotateAroundAxis(Transform* _pOut, const D3DXVECTOR3* _pkAxis, float _fRadians)
	{
		D3DXQUATERNION _rotQ;
		D3DXQuaternionRotationAxis(&_rotQ, _pkAxis, _fRadians);//MF: D3DX Usage

		_pOut->rotation *= _rotQ; //Todo: check if needs other way around
	}

	/**
	* Rotate transform around a point and provided axis
	* @author	Serge Radinovich
	* @param	_pOut			New Transform result
	* @param	_pkPoint		Point around which to rotate transform
	* @param	_pkAxis			Axis around which to rotate transform
	* @param	_fRadians		Radians to rotate by
	*/
	void Transform::rotateAroundPoint(Transform* _pOut, const D3DXVECTOR3* _pkPoint, const D3DXVECTOR3* _pkAxis, float _fRadians)
	{
		D3DXVECTOR3 _translateVec = *_pkPoint - _pOut->translation;
		D3DXVECTOR4 _temp4D;

		//Translate
		_pOut->translation += _translateVec;

		//Rotate
		D3DXQUATERNION _rotQ;
		D3DXQuaternionRotationAxis(&_rotQ, _pkAxis, _fRadians); //Get rotation Quat to rotate obj and trans vec

		_pOut->rotation *= _rotQ;							//Rotate the object
		vecXquat(&_translateVec, &_rotQ, &_translateVec);   //Rotate translation vector

		//Translate again
		_pOut->translation -= _translateVec;

	}

	/**
	* Rotate transform toward a rotation represented by forward vector provided
	* @author	Serge Radinovich
	* @param	_pOut				New Transform result
	* @param	_pkNewForward		New forward to rotate toward
	* @param	_fInterpFactor		Rate of interpolation
	*/
	void Transform::rotateToward(Transform* _pOut, const D3DXVECTOR3* _pkNewForward, float _fInterpFactor)
	{
		//Create the target Quaternion to be SLERP'd toward
		Transform _targetTransform;
		setForward(&_targetTransform, _pkNewForward);
		D3DXQUATERNION _targetQ;
		D3DXQuaternionRotationMatrix(&_targetQ, _targetTransform.matrix());

		D3DXQuaternionSlerp(&_pOut->rotation, &_pOut->rotation, &_targetQ, _fInterpFactor);//MF: D3DX Usage
	}

