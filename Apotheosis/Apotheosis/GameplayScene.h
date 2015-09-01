#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__

#include "Util.h"
#include "IGameScene.h"
#include "Player.h"
#include "Platform.h"


class GameplayScene : public IGameScene
{

public: //Construction

	GameplayScene(){}
	~GameplayScene(){}

	GameplayScene(const GameplayScene&) = delete;
	GameplayScene& operator=(const GameplayScene&) = delete;

private: //Data

	array<Platform, 4> m_platforms;

	//Player m_player;
	array<Player, 4> m_players;

public: //Interface

	void init() override
	{

		for (UINT i = 0; i < 4; ++i)
			m_platforms[i].init(b2Vec2(-25.0f + 10.0f*i, -2.0f), 3.0f, 1.0f);

		for (UINT i = 0; i < m_players.size(); ++i)
			m_players[i].init(b2Vec2(-2.0f + 2.0f*i, 5.0f), 1.0f, 1.0f);
			

	}
	void update(float _fDeltaTime) override
	{
		for (auto& rPlatform : m_platforms)
			rPlatform.update(_fDeltaTime);

		for (auto& _rPlayer : m_players)
			_rPlayer.update(_fDeltaTime);
	}

	void setActive(bool _bActive) override
	{
		for (auto& rPlatform : m_platforms)
			rPlatform.setActive(_bActive);

		for (auto& _rPlayer : m_players)
			_rPlayer.setActive(_bActive);
	}

};




#endif //__GAMEPLAY_SCENE_H__