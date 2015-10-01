#include "PhysicsSystem.h"


PhysicsSystem* PhysicsSystem::s_pInstance = nullptr; ///Singleton pointer


PhysicsSystem::PhysicsSystem()
	:m_world(b2Vec2(0.0f, -10.0f)) //Construct the world with gravity
{

}

PhysicsSystem::~PhysicsSystem()
{

}

void PhysicsSystem::init(float _fMinX, float _fMaxX, float _fMinY, float _fMaxY)
{
	array<b2Body*, 4> _walls{};
	array<b2Vec2, 4> _positions{};
	array<b2Vec2, 4> _dimensions{};
	//Vertical
	_positions[0] = b2Vec2(_fMaxY, 0); _dimensions[0] = b2Vec2(0.5f, 100.0f);
	_positions[1] = b2Vec2(_fMinY, 0);  _dimensions[1] = b2Vec2(0.5f, 100.0f);
	//Horizontal
	_positions[2] = b2Vec2(0, _fMinX);  _dimensions[2] = b2Vec2(100.0f, 0.5f);
	_positions[3] = b2Vec2(0, _fMaxX); _dimensions[3] = b2Vec2(100.0f, 0.5f);

	b2BodyDef _groundBodyDef;
	b2Body* _pGroundBody{};
	b2PolygonShape _groundBox;
	for (int i = 0; i < 4; ++i)
	{		
		_groundBodyDef.position.Set(_positions[i].x, _positions[i].y);

		_pGroundBody = m_world.CreateBody(&_groundBodyDef);

		_groundBox.SetAsBox(_dimensions[i].x, _dimensions[i].y);

		_pGroundBody->CreateFixture(&_groundBox, 0.0f);
	}
	


	
}

void PhysicsSystem::shutDown()
{
	SAFE_DELETE(s_pInstance);
}

void PhysicsSystem::update(float _fDeltaTime)
{
	m_world.Step(_fDeltaTime, m_iVelocityIterations, m_iPositionIterations);
}

b2Body* PhysicsSystem::createBody(b2BodyType _eType, const b2Vec2& _rkPosition)
{
	b2BodyDef bodyDef;
	bodyDef.type = _eType;
	bodyDef.position.Set(_rkPosition.x, _rkPosition.y);
	bodyDef.fixedRotation = true;
	return s_pInstance->m_world.CreateBody(&bodyDef);
}

void PhysicsSystem::addFixtureToBody(b2Body* _pBody, const b2PolygonShape& _rkShape, b2FixtureDef& _rkFixtureDef)
{

	_rkFixtureDef.shape = &_rkShape;


	_pBody->CreateFixture(&_rkFixtureDef);
}