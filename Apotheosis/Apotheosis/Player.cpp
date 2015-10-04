#include "Player.h"

UINT Player::s_iPlayerCount = 0;

Player::Player()
{
	m_iPlayerID = s_iPlayerCount++;
}

Player::~Player()
{
}


void Player::init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY)
{
	//Rendering
	IActor::initializeRendering("fire", ".bmp", 22, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);
	//IActor::addTexture("WireFence.dds");
	IActor::initializeRendering("playerJump", ".dds", 2, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);
	//IActor::addTexture("water2.dds");
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

void Player::update(float _fDeltaTime)
{
	//IActor::animate(_fDeltaTime);

	//float _fAngle = m_rigidBodies.back()->GetAngle();
	//Transform::rotateAroundAxis(&m_transform, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), _fAngle);
	m_transform.translation.x = m_rigidBodies.back()->GetPosition().x;
	m_transform.translation.y = m_rigidBodies.back()->GetPosition().y;
	m_renderTasks[m_iCurrTaskIndex].transformMtx = *m_transform.matrix();

	D3DXVECTOR2 _frameTranslation = D3DXVECTOR2(0.0f, 0.0f);
	if (InputHandler::getInstance()->handleObjectTranslation(m_iPlayerID, _frameTranslation, _fDeltaTime))
	{
		if (!m_rigidBodies.back()->IsAwake())
			m_rigidBodies.back()->SetAwake(true);

		b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
		_b2Vel.x = 0.0f;
		m_rigidBodies.back()->SetLinearVelocity(_b2Vel);

		b2Vec2 _b2Trans;
		_b2Trans.x = _frameTranslation.x * 10;
		_b2Trans.y = 0;

		//m_rigidBodies.back()->ApplyForceToCenter(_b2Trans, false);
		m_rigidBodies.back()->ApplyLinearImpulse(_b2Trans, b2Vec2(0.0f, 0.0f), false);
	}
	if (InputHandler::getInstance()->handleJump(m_iPlayerID, _fDeltaTime))
	{
		if (!m_rigidBodies.back()->IsAwake())
			m_rigidBodies.back()->SetAwake(true);

		IActor::setCurrState(1);
		b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
		_b2Vel.y = 0.0f;
		m_rigidBodies.back()->SetLinearVelocity(_b2Vel);
		b2Vec2 _b2Trans = b2Vec2(0.0f, 50.0f);
		m_rigidBodies.back()->ApplyLinearImpulse(_b2Trans, b2Vec2(0.0f, 0.0f), false);
	}

	if (m_iCurrTaskIndex == 1 && fabsf(m_rigidBodies.back()->GetLinearVelocity().y) < 0.01f && m_rigidBodies.back()->GetContactList() != nullptr)
		IActor::setCurrState(0);
}

void Player::respawn()
{
	setPosition(m_spawnPos.x, m_spawnPos.y);
}