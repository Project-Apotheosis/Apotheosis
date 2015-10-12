#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__

#include "Util.h"
#include "IGameScene.h"
#include "Player.h"
#include "Platform.h"


class GameplayScene : public IGameScene
{

public: //Construction

	GameplayScene();
	~GameplayScene();

	GameplayScene(const GameplayScene&) = delete;
	GameplayScene& operator=(const GameplayScene&) = delete;

private: //Data

	bool m_bSaveLevelOnExit{};

	array<Platform, 50> m_platforms;

	UINT m_iInactivePlatformCount{}; //Set from loading xml

	//Player m_player;
	array<Player, 4> m_players;

public: //Interface

	void init() override;

	void update(float _fDeltaTime) override;

	void setActive(bool _bActive) override;

	bool sceneEnding(E_SCENE& _reNextScene) override;

	//Callbacks
	void playerAttackCallback(Player& _rkAttackingPlayer);
	void playerChannelSuccessCallback();
};




#endif //__GAMEPLAY_SCENE_H__