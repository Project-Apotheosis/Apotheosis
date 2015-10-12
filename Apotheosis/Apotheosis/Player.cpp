#include "Player.h"
#include "AudioManager.h"

UINT Player::s_iPlayerCount = 0;
float Player::s_kfAttackAnimTime = 0.35f;
float Player::s_kfDashAnimTime = 0.09f;
float Player::s_kfDeathAnimTime = 0.25f;

D3DXVECTOR4 g_playerColors[4] = {   
									{ 176.0f / 255.0f, 42.0f / 255.0f, 42.0f / 255.0f, 1.0f },
									{ 176.0f / 255.0f, 152.0f / 255.0f, 42.0f / 255.0f, 1.0f },
									{ 109.0f / 255.0f, 176.0f / 255.0f, 42.0f / 255.0f, 1.0f },
									{ 42.0f / 255.0f, 97.0f / 255.0f, 176.0f / 255.0f, 1.0f },
								};


Player::Player()
{
	m_iPlayerID = s_iPlayerCount++;



}

Player::~Player()
{
}

enum PlayAnim : int { Walk, Jump, Fall, Attack, Channel, Death, Walk_, Jump_, Fall_, Attack_, Channel_, Death_};

void Player::init(function<void(Player&)> _callback, const b2Vec2& _rkPosition, float _fDimX, float _fDimY)
{
	m_attackCallback = _callback;
	

	//Rendering
	m_playerScore.init(m_iPlayerID);
	//Non-Skeleton
	//Run
	IActor::initializeRendering("ApotheosisKnight_Walk_", ".png", 10, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Jump
	IActor::initializeRendering("ApotheosisKnight_Jump_", ".png", 1, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Fall
	IActor::initializeRendering("ApotheosisKnight_Fall_", ".png", 1, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Attack
	IActor::initializeRendering("ApotheosisKnight_Attack_", ".png", 8, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Channel
	IActor::initializeRendering("ApotheosisKnight_Channel_", ".png", 9, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Death
	IActor::initializeRendering("ApotheosisSkeleton_Death_", ".png", 1, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Skeleton
	//Run
	IActor::initializeRendering("ApotheosisSkeleton_Walk_", ".png", 10, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Jump
	IActor::initializeRendering("ApotheosisSkeleton_Jump_", ".png", 1, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Fall
	IActor::initializeRendering("ApotheosisSkeleton_Fall_", ".png", 1, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Attack
	IActor::initializeRendering("ApotheosisSkeleton_Attack_", ".png", 8, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Channel
	IActor::initializeRendering("ApotheosisKnight_Channel_", ".png", 9, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Death
	IActor::initializeRendering("ApotheosisSkeleton_Death_", ".png", 1, "texAlpha.fx", "TexAlphaTech", _fDimX, _fDimY);

	//Set player unique color
	for (auto& rTask : m_renderTasks)
	{
		rTask.playerColor = g_playerColors[m_iPlayerID];
	}


	IActor::setCurrState(Walk);

	//Physics
	m_rigidBodies.push_back(PhysicsSystem::createBody(b2BodyType::b2_dynamicBody, _rkPosition));

	b2ChainShape dynamicBox;

	b2CircleShape circle;
	circle.m_radius = _fDimX;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;

	PhysicsSystem::addFixtureToBody(m_rigidBodies.back(), circle, fixtureDef);
}

void Player::update(float _fDeltaTime)
{

	updateTransform();

	updateAnimations();

	//Update death
	if (m_fDeathAnimTick >= 0.0f)
	{
		setCurrState(Death);
		m_fDeathAnimTick -= _fDeltaTime;
		if (m_fDeathAnimTick < 0.0f)
		{
			//?
		}
		return;
	}

	//Update dash
	if (m_fDashAnimTick >= 0.0f)
	{
		m_fDashAnimTick -= _fDeltaTime;
		if (m_fDashAnimTick < 0.0f)
		{
			m_rigidBodies.front()->SetLinearVelocity(b2Vec2(0, 0));
		}
		return;
	}
	
	//Update attack
	if (m_fAttackAnimTick >= 0.0f)
	{
		//m_rigidBodies.front()->SetLinearVelocity(b2Vec2(0, 0));
		m_fAttackAnimTick -= _fDeltaTime;

		return;
	}

	//Update channel
	if (m_bHoldingChannel && m_iSoulCount == 4 && m_rigidBodies.front()->GetLinearVelocity().x < 0.01f && m_rigidBodies.front()->GetLinearVelocity().y < 0.01f)
	{
		m_fChannelTick += _fDeltaTime;
		if (m_fChannelTick > 3.0f)
		{
			m_fChannelTick = 0.0f;
			m_iSoulCount = 1;
			m_playerScore.updateSkullLevel(++m_iPlayerSkullLevel);
			m_playerScore.updatePoints(m_iSoulCount);
		}
	}
	else
	{
		m_fChannelTick = 0.0f;
	}

	handleInput(_fDeltaTime);






}

void Player::stealSouls(UINT _iSoulCount)
{
	m_iSoulCount += _iSoulCount;

	switch (m_iPlayerSkullLevel)
	{
	case 0:
		if (m_iSoulCount > 4)
			m_iSoulCount = 4;
		break;

	case 1: 
		if (m_iSoulCount > 7)
			m_iSoulCount = 7;
		break;

	case 2:
		if (m_iSoulCount > 10)
			m_iSoulCount = 10;
		break;
	}

	m_playerScore.updatePoints(m_iSoulCount);
}

void Player::kill(b2Vec2 _force)
{
	AudioManager::PlayTakeDamage();
	m_iSoulCount = 0;
	m_bSkeleton = true;
	_force.x *= 10000.0f;
	_force.y *= 10000.0f;
	m_rigidBodies.back()->ApplyLinearImpulse(_force, b2Vec2(0.0f, 0.0f), true);
	
	setCurrState(Death);
	m_fDeathAnimTick = s_kfDeathAnimTime;

	
	m_playerScore.updatePoints(m_iSoulCount);
}

void Player::respawn()
{
	setPosition(m_spawnPos.x, m_spawnPos.y);
}


void Player::updateAnimations()
{
	UINT iAnimOffset = m_iSoulCount > 0 ? 0 : Walk_;
	
	//Update UVs based on player direction
	m_renderTasks[m_iCurrTaskIndex].bDefaultUVs = m_currentDirection == PlayerDirection::right ? true : false;

	//Channel animation
	if (m_bHoldingChannel && m_fChannelTick > 0.0f)
		IActor::setCurrState(Channel);
	else if (fabsf(m_rigidBodies.back()->GetLinearVelocity().y) < 0.01f && m_rigidBodies.back()->GetContactList() != nullptr) //Run animation 
		IActor::setCurrState(Walk + iAnimOffset);		

	//Jumping up animation
	if (m_rigidBodies.back()->GetLinearVelocity().y > 0.01f)
		IActor::setCurrState(Jump + iAnimOffset);

	//Falling animation
	if (m_rigidBodies.back()->GetLinearVelocity().y < 0.01f && m_rigidBodies.back()->GetContactList() == nullptr)
		IActor::setCurrState(Fall + iAnimOffset);

	//Attack anim
	if (m_fAttackAnimTick > 0.0f && m_iCurrTaskIndex != 3)
		setCurrState(Attack + iAnimOffset);

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
	//Inititiate Attack
	if ( InputHandler::getInstance()->handleAttack(m_iPlayerID, _fDeltaTime))
	{
		AudioManager::PlayMelee();
		m_fAttackAnimTick = s_kfAttackAnimTime; //Enable attack state
		m_attackCallback(*this);
	}
	
	

	//Channel
	if (InputHandler::getInstance()->handleChannel(m_iPlayerID))
	{
		m_bHoldingChannel = true;	
	}
	else
	{
		m_bHoldingChannel = false;
	}
		
	//Initiate Dash
	int iDashDir = 0;
	if (InputHandler::getInstance()->handleDash(m_iPlayerID, _fDeltaTime, iDashDir))
	{
		AudioManager::PlaySlide();
		m_fDashAnimTick = s_kfDashAnimTime; //Enable dash state;
		m_rigidBodies.front()->SetLinearVelocity(b2Vec2(200 * iDashDir, 0));
	}
	
	D3DXVECTOR2 _frameTranslation = D3DXVECTOR2(0.0f, 0.0f);
	if (InputHandler::getInstance()->handleObjectTranslation(m_iPlayerID, _frameTranslation, _fDeltaTime))
	{
		//Player default faces right
		m_currentDirection = _frameTranslation.x >= 0.0f ? PlayerDirection::right : PlayerDirection::left;


		if (!m_rigidBodies.back()->IsAwake())
			m_rigidBodies.back()->SetAwake(true);

		b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
		_b2Vel.x *= 0.5f;
		m_rigidBodies.back()->SetLinearVelocity(_b2Vel);

		b2Vec2 _b2Trans;
		_b2Trans.x = _frameTranslation.x * 200;
		_b2Trans.y = 0;

		//m_rigidBodies.back()->ApplyForceToCenter(_b2Trans, false);
		m_rigidBodies.back()->ApplyLinearImpulse(_b2Trans, b2Vec2(0.0f, 0.0f), false);
	}
	else
	{
		
		b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
		_b2Vel.x *= 0.85f;
		m_rigidBodies.back()->SetLinearVelocity(_b2Vel);
	}


	if (InputHandler::getInstance()->handleJump(m_iPlayerID, _fDeltaTime))
	{
		
		if (!m_rigidBodies.back()->IsAwake())
			m_rigidBodies.back()->SetAwake(true);

		AudioManager::PlayJump();
		b2Vec2 _b2Vel = m_rigidBodies.back()->GetLinearVelocity();
		_b2Vel.y = 0.0f;
		m_rigidBodies.back()->SetLinearVelocity(_b2Vel);
		b2Vec2 _b2Trans = b2Vec2(0.0f, 1000.0f);
		m_rigidBodies.back()->ApplyLinearImpulse(_b2Trans, b2Vec2(0.0f, 0.0f), false);
	}

}



//Player score

void Player::Score::init(UINT _iPlayerID)
{
	Transform winSkullTform = m_transform;
	
	float fEdgeX = 55.0f;
	float fEdgeY = 35.0f;

	float fWinSkullDeltaY = 5.0f;

	//Set up translation
	switch (_iPlayerID)
	{
	case 0:
		m_transform.translation = D3DXVECTOR3(-fEdgeX, 32.0f, -5.0f);
		winSkullTform.translation = D3DXVECTOR3(-fEdgeX, 32.0f - fWinSkullDeltaY, -5.0f);
		break;
		

	case 1:
		m_transform.translation = D3DXVECTOR3(fEdgeX, 32.0f, -5.0f);
		winSkullTform.translation = D3DXVECTOR3(fEdgeX, 32.0f - fWinSkullDeltaY, -5.0f);
		break;
		
	case 2:
		m_transform.translation = D3DXVECTOR3(-fEdgeX, -32.0f, -5.0f);
		winSkullTform.translation = D3DXVECTOR3(-fEdgeX, -32.0f - fWinSkullDeltaY, -5.0f);
		break;

	case 3:
		m_transform.translation = D3DXVECTOR3(fEdgeX, -32.0f, -5.0f);
		winSkullTform.translation = D3DXVECTOR3(fEdgeX, -32.0f - fWinSkullDeltaY, -5.0f);
		break;
	}

	float fSize = 2.0f;

	//1
	IActor::initializeRendering("ApotheosisUISkull_", ".png", 1, "texAlpha.fx", "TexAlphaTech", fSize, fSize);
	m_renderTasks.back().transformMtx = *m_transform.matrix();
	m_transform.translation.x += 4.0f;
	IActor::initializeRendering("ApotheosisUISkull_", ".png", 1, "texAlpha.fx", "TexAlphaTech", fSize, fSize);
	m_renderTasks.back().transformMtx = *m_transform.matrix();
	m_transform.translation.x += 4.0f;
	IActor::initializeRendering("ApotheosisUISkull_", ".png", 1, "texAlpha.fx", "TexAlphaTech", fSize, fSize);
	m_renderTasks.back().transformMtx = *m_transform.matrix();
	


	IActor::initializeRendering("ApotheosisUIGhostSkull_", ".png", 1, "texAlpha.fx", "TexAlphaTech", fSize, fSize);
	m_renderTasks.back().transformMtx = *winSkullTform.matrix();
	winSkullTform.translation.x += 4.0f;
	IActor::initializeRendering("ApotheosisUIGhostSkull_", ".png", 1, "texAlpha.fx", "TexAlphaTech", fSize, fSize);
	m_renderTasks.back().transformMtx = *winSkullTform.matrix();
	winSkullTform.translation.x += 4.0f;
	IActor::initializeRendering("ApotheosisUIGhostSkull_", ".png", 1, "texAlpha.fx", "TexAlphaTech", fSize, fSize);
	m_renderTasks.back().transformMtx = *winSkullTform.matrix();


	for (auto& rTask : m_renderTasks)
		rTask.rendering = false;
}

void Player::Score::updateSkullLevel(UINT _iPlayerSkullLevel)
{
	m_renderTasks[_iPlayerSkullLevel + 2].rendering = true;
}

void Player::Score::updatePoints(UINT _iPlayerSoulCount)
{
	
	if (_iPlayerSoulCount < 2)
	{
		for (UINT i = 0; i < 3; ++i)
			m_renderTasks[i].rendering = false;
	}
	else if (_iPlayerSoulCount >= 2 && _iPlayerSoulCount < 5)
	{
		m_renderTasks[_iPlayerSoulCount-2].rendering = true;
	}
}



void Player::Score::update(float _fDeltaTime)
{}