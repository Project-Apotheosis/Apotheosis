#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "IActor.h"
#include "InputHandler.h"
#include <functional>

class Player : public IActor
{
	class Score : public IActor
	{
	public:
		void init(UINT _iPlayerID);

		void updatePoints(UINT _iPlayerSoulCount);
		void updateSkullLevel(UINT _iPlayerSkullLevel);

		void update(float _fDeltaTime);

		
	};
	
	
	
	
	
	enum PlayerDirection : int { left = -1, right = 1};


public: //Construction

	Player();
	~Player();

private: //Data

	Score m_playerScore;

	static UINT s_iPlayerCount;
	UINT m_iPlayerID;

	D3DXVECTOR3 m_spawnPos; //Manipulated through GameplayScene

	PlayerDirection m_currentDirection = PlayerDirection::right;

	//Attack timing
	static float s_kfAttackAnimTime;
	float m_fAttackAnimTick = 0.0f;
	//Dash timing
	static float s_kfDashAnimTime;
	float m_fDashAnimTick = 0.0f;
	//Death timing
	static float s_kfDeathAnimTime;
	float m_fDeathAnimTick = 0.0f;


	function<void(Player&)> m_attackCallback;
	function<void()> m_channelCallback;

	bool m_bSkeleton{};
	UINT m_iSoulCount{ 1 };

	float m_fChannelTick{};
	bool m_bHoldingChannel{};
	bool m_bChannelStarted{};
	UINT m_iPlayerSkullLevel{};

public: //Interface

	UINT getPlayerID() const { return m_iPlayerID; }
	UINT getSoulCount() const { return m_iSoulCount; }

	static UINT playerCount(){ return s_iPlayerCount; }

	void init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY);
	void initCallbacks(function<void(Player&)> _attack, function<void()> _channel);
	void update(float _fDeltaTime) override;

	b2Vec2 getSpawnPosition() const { return{ m_spawnPos.x, m_spawnPos.y }; }
	void setSpawnPosition(float _fX, float _fY) { m_spawnPos.x = _fX; m_spawnPos.y = _fY; }

	void respawn();

	//Called from callback (member function of GameplayScene
	void stealSouls(UINT _iSoulCount);
	//Called from callback (member function of GameplayScene
	void kill(b2Vec2 _force);

private: //Internals

	void updateTransform();
	void updateAnimations();
	void handleInput(float _fDeltaTime);

};

#endif //__PLAYER_H__