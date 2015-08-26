#include "IActor.h"

class Platform : public IActor
{
public: //Construction

	Platform()
	{
	}

	~Platform()
	{
	}


private: //Data


public: //Interface

	void init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY) override
	{
		IActor::initializeRendering("WireFence.dds", "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

		m_rigidBodies.push_back(PhysicsSystem::createBody(b2BodyType::b2_staticBody, _rkPosition));

		b2PolygonShape _box;
		_box.SetAsBox(_fDimX, _fDimY);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &_box;

		PhysicsSystem::addFixtureToBody(m_rigidBodies.back(), _box, fixtureDef);
		

		m_transform.translation.x = m_rigidBodies.back()->GetPosition().x;
		m_transform.translation.y = m_rigidBodies.back()->GetPosition().y;
		m_renderTasks.back().transformMtx = *m_transform.matrix();
	}
	
	void update(float _fDeltaTime) override
	{
		//float _fAngle = m_rigidBodies.back()->GetAngle();
		
	}
};