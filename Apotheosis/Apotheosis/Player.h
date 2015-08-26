#include "IActor.h"
#include "InputHandler.h"

class Player : public IActor
{
public: //Construction

	Player()
	{
	}

	~Player()
	{
	}


private: //Data


public: //Interface

	void init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY) override
	{
		//Rendering
		IActor::initializeRendering("grass.dds", "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);
		IActor::addTexture("WireFence.dds");
		IActor::initializeRendering("water1.dds", "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);
		IActor::addTexture("water2.dds");
		IActor::setCurrState(0);

		//Physics
		m_rigidBodies.push_back(PhysicsSystem::createBody(b2BodyType::b2_dynamicBody, _rkPosition));

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(_fDimX, _fDimY);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;

		PhysicsSystem::addFixtureToBody(m_rigidBodies.back(), dynamicBox, fixtureDef);

	}

	void update(float _fDeltaTime) override
	{
		IActor::animate(_fDeltaTime);

		//float _fAngle = m_rigidBodies.back()->GetAngle();
		//Transform::rotateAroundAxis(&m_transform, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), _fAngle);
		m_transform.translation.x = m_rigidBodies.back()->GetPosition().x;
		m_transform.translation.y = m_rigidBodies.back()->GetPosition().y;
		m_renderTasks[m_iCurrTaskIndex].transformMtx = *m_transform.matrix();

		D3DXVECTOR2 _frameTranslation = D3DXVECTOR2(0.0f,0.0f);
		if (InputHandler::getInstance()->handleObjectTranslation(_frameTranslation, _fDeltaTime))
		{
			b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
			_b2Vel.x = 0.0f;
			m_rigidBodies.back()->SetLinearVelocity(_b2Vel);

			b2Vec2 _b2Trans;
			_b2Trans.x = _frameTranslation.x * 10;
			_b2Trans.y = 0;

			//m_rigidBodies.back()->ApplyForceToCenter(_b2Trans, false);
			m_rigidBodies.back()->ApplyLinearImpulse(_b2Trans, b2Vec2(0.0f, 0.0f), false);
		}
		if (InputHandler::getInstance()->handleJump(_fDeltaTime))
		{
			IActor::setCurrState(1);
			b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
			_b2Vel.y = 0.0f;
			m_rigidBodies.back()->SetLinearVelocity(_b2Vel);
			b2Vec2 _b2Trans = b2Vec2(0.0f, 50.0f);
			m_rigidBodies.back()->ApplyLinearImpulse(_b2Trans, b2Vec2(0.0f, 0.0f), false);
		}

		if(m_iCurrTaskIndex == 1 && fabsf(m_rigidBodies.back()->GetLinearVelocity().y) < 0.01f && m_rigidBodies.back()->GetContactList() != nullptr)
			IActor::setCurrState(0);
	}
};