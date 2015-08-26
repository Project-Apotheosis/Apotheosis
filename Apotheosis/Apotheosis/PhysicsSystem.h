#ifndef __TYRO_PHYSICS_SYSTEM_H__
#define __TYRO_PHYSICS_SYSTEM_H__

#include "Util.h"

#include "Box2D.h"



class PhysicsSystem 
{

private: //Construction

	PhysicsSystem();
	static PhysicsSystem* s_pInstance;

public:

	~PhysicsSystem();

	//Singleton 	
	static PhysicsSystem* getInstance()
	{
		if (!s_pInstance)
			s_pInstance = new PhysicsSystem();

		return s_pInstance;
	}

	void init(float _fMinX, float _fMaxX, float _fMinY, float _fMaxY);
	void shutDown();

	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem& operator=(const PhysicsSystem&) = delete;

private: //Data

	int32	m_iVelocityIterations = 6;
	int32	m_iPositionIterations = 2;

	b2World m_world;


public: //Interface

	void update(float _fDeltaTime);

	static b2Body* createBody(b2BodyType _eType, const b2Vec2& _rkPosition);

	static void addFixtureToBody(b2Body* _pBody, const b2PolygonShape& _rkShape, b2FixtureDef& _rkFixtureDef);
};




#endif //__TYRO_PHYSICS_SYSTEM_H__