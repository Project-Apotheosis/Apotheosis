#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "IActor.h"
#include "InputHandler.h"
#include <functional>

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

	//Attack timing
	static float s_kfAttackAnimTime;
	float m_fAttackAnimTick = s_kfAttackAnimTime;
	//Dash timing
	static float s_kfDashAnimTime;
	float m_fDashAnimTick = s_kfDashAnimTime;

	function<void(const Player&)> m_attackCallback;
	
public: //Interface

	UINT getPlayerID() const { return m_iPlayerID; }

	static UINT playerCount(){ return s_iPlayerCount; }

	void init(function<void(const Player&)> _callback, const b2Vec2& _rkPosition, float _fDimX, float _fDimY);

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