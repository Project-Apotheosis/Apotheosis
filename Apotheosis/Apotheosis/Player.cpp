#include "Player.h"

UINT Player::s_iPlayerCount = 0;
float Player::s_kfAttackAnimTime = 0.35f;


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
	//Run
	IActor::initializeRendering("KnightRunCycle_Animation", ".png", 10, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Jump
	IActor::initializeRendering("ApotheosisKnight_Jump", ".png", 1, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Fall
	IActor::initializeRendering("ApotheosisKnight_Fall", ".png", 1, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Attack
	IActor::initializeRendering("ApotheosisKnight_Attack", ".png", 4, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

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
	
	

	handleInput(_fDeltaTime);

	updateTransform();

	updateAnimations();





}

void Player::respawn()
{
	setPosition(m_spawnPos.x, m_spawnPos.y);
}


void Player::updateAnimations()
{
	//Update UVs based on player direction
	m_renderTasks[m_iCurrTaskIndex].bDefaultUVs = m_currentDirection == PlayerDirection::right ? true : false;

	//Run animation
	if (m_iCurrTaskIndex != 0 && fabsf(m_rigidBodies.back()->GetLinearVelocity().y) < 0.01f && m_rigidBodies.back()->GetContactList() != nullptr)
		IActor::setCurrState(0);

	//Jumping up animation
	if (m_iCurrTaskIndex != 1 && m_rigidBodies.back()->GetLinearVelocity().y > 0.01f)
		IActor::setCurrState(1);

	//Falling animation
	if (m_iCurrTaskIndex != 2 && m_rigidBodies.back()->GetLinearVelocity().y < 0.01f && m_rigidBodies.back()->GetContactList() == nullptr)
		IActor::setCurrState(2);

	if (m_fAttackAnimTick > 0.0f && m_iCurrTaskIndex != 3)
		setCurrState(3);

}
void Player::updateTransform()
{
	//float _fAngle = m_rigidBodies.back()->GetAngle();
	//Transform::rotateAroundAxis(&m_transform, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), _fAngle);
	m_transform.translation.x = m_rigidBodies.back()->GetPosition().x;
	m_transform.translation.y = m_rigidBodies.back()->GetPosition().y;
	for (auto& rRenderTask : m_renderTasks)
		rRenderTask.transformMtx = *m_transform.matrix();
}

void Player::handleInput(float _fDeltaTime)
{
	//Attack
	if ( InputHandler::getInstance()->handleAttack(m_iPlayerID, _fDeltaTime))
	{
		m_fAttackAnimTick = s_kfAttackAnimTime; //Enable attack state
		
	}
	if (m_fAttackAnimTick >= 0.0f)
	{
		m_rigidBodies.front()->SetLinearVelocity(b2Vec2(0, 0));
		m_fAttackAnimTick -= _fDeltaTime;
		return;
	}
		
	
	
	D3DXVECTOR2 _frameTranslation = D3DXVECTOR2(0.0f, 0.0f);
	if (InputHandler::getInstance()->handleObjectTranslation(m_iPlayerID, _frameTranslation, _fDeltaTime))
	{
		//Player default faces right
		m_currentDirection = _frameTranslation.x >= 0.0f ? PlayerDirection::right : PlayerDirection::left;


		if (!m_rigidBodies.back()->IsAwake())
			m_rigidBodies.back()->SetAwake(true);

		b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
		_b2Vel.x = 0.0f;
		m_rigidBodies.back()->SetLinearVelocity(_b2Vel);

		b2Vec2 _b2Trans;
		_b2Trans.x = _frameTranslation.x * 400;
		_b2Trans.y = 0;

		//m_rigidBodies.back()->ApplyForceToCenter(_b2Trans, false);
		m_rigidBodies.back()->ApplyLinearImpulse(_b2Trans, b2Vec2(0.0f, 0.0f), false);
	}

	if (InputHandler::getInstance()->handleJump(m_iPlayerID, _fDeltaTime))
	{
		
		if (!m_rigidBodies.back()->IsAwake())
			m_rigidBodies.back()->SetAwake(true);


		b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
		_b2Vel.y = 0.0f;
		m_rigidBodies.back()->SetLinearVelocity(_b2Vel);
		b2Vec2 _b2Trans = b2Vec2(0.0f, 1000.0f);
		m_rigidBodies.back()->ApplyLinearImpulse(_b2Trans, b2Vec2(0.0f, 0.0f), false);
	}

}