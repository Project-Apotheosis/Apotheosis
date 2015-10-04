// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : IActor.h
// Description :  IActor h
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#ifndef __ACTOR_H__
#define __ACTOR_H__


#include "Util.h"
#include "Transform.h"
#include "DX10Renderer.h"
#include "PhysicsSystem.h"
#include <queue>

class IActor
{
public: //Construction

	IActor();

	virtual	~IActor();

	virtual void init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY) = 0;

protected: //Data

	bool m_bActive{ true };

	Transform		m_transform;
	vector<RenderTask2D>	m_renderTasks; //Rendering components
	vector<b2Body*>			m_rigidBodies; //Physics components
	vector<queue<ID3D10ShaderResourceView*>> m_textureAnimQueues{};
	float m_fAnimRate{ 1.0f };
	float m_fLastAnim{};

	UINT m_iCurrTaskIndex{ 0 };

public: //Interface

	bool isActive() { return m_bActive; }
	b2Vec2 getPosition();

	Transform& transform() { return m_transform; }
	vector<RenderTask2D>& renderTask() { return m_renderTasks; }
	vector<b2Body*>& rigidBodies() { return m_rigidBodies; }

	void initializeRendering(const string& _rkTexture, const string& _rkSuffix, const UINT _kiSpriteCount, const string& _rkFX, const string& _rkTech, float _fDimX, float _fDimY);

	/**
	*	Update every frame. Use this function to manipulate components
	*	@param _fDeltaTime	Change in time since last frame
	*/
	virtual void update(float _fDeltaTime) = 0;

	void setActive(bool _bActive);

	void setPosition(float _fX, float _fY);


protected:

	void setCurrState(UINT _iIndex);
	//void animate(float _fDeltaTime);



};



#endif //__ACTOR_H__

