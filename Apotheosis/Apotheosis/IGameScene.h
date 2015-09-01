#ifndef __IGAME_SCENE_H__
#define __IGAME_SCENE_H__

#include "Util.h"
#include "IActor.h"

class IGameScene
{

public:

	IGameScene(){}
	virtual ~IGameScene(){}

	IGameScene(const IGameScene&) = delete;
	IGameScene& operator=(const IGameScene&) = delete;

protected: //Data



public: //Interface

	virtual void init() = 0;
	virtual void update(float _fDeltaTime) = 0;
	virtual void setActive(bool) = 0;

};




#endif //__IGAME_SCENE_H__