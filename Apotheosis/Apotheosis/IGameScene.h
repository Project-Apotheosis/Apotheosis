#ifndef __IGAME_SCENE_H__
#define __IGAME_SCENE_H__

#include "IActor.h"
#include "Util.h"


class IGameScene
{

public:

	IGameScene(){}
	virtual ~IGameScene(){}

	IGameScene(const IGameScene&) = delete;
	IGameScene& operator=(const IGameScene&) = delete;

protected: //Data

	//Used via callbacks for buttons
	bool m_bSceneEnding{};
	E_SCENE m_nextSceneSelected = S_GAMEPLAY;


public: //Interface

	virtual void init() = 0;
	virtual void update(float _fDeltaTime) = 0;
	virtual void setActive(bool) = 0;
	virtual bool sceneEnding(E_SCENE& _reNextScene) = 0;

};




#endif //__IGAME_SCENE_H__