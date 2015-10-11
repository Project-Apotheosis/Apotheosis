#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "IActor.h"
#include "InputHandler.h"

class Player : public IActor
{

	enum PlayerDirection : int { left = -1, right = 1};

public: //Construction

	Player();
	~Player();


private: //Data

	static UINT s_iPlayerCount;
	UINT m_iPlayerID;

	D3DXVECTOR3 m_spawnPos; //Manipulated through GameplayScene

	PlayerDirection m_currentDirection = PlayerDirection::right;

	static float s_kfAttackAnimTime;
	float m_fAttackAnimTick = s_kfAttackAnimTime;
	
public: //Interface

	static UINT playerCount(){ return s_iPlayerCount; }

	void init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY) override;

	void update(float _fDeltaTime) override;

	b2Vec2 getSpawnPosition() const { return{ m_spawnPos.x, m_spawnPos.y }; }
	void setSpawnPosition(float _fX, float _fY) { m_spawnPos.x = _fX; m_spawnPos.y = _fY; }

	void respawn();

private: //Internals

	void updateTransform();
	void updateAnimations();
	void handleInput(float _fDeltaTime);

};

#endif //__PLAYER_H__