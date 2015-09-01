#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "IActor.h"
#include "InputHandler.h"

class Player : public IActor
{
public: //Construction

	Player();
	~Player();


private: //Data

	static UINT s_iPlayerCount;
	UINT m_iPlayerID;

public: //Interface

	static UINT playerCount(){ return s_iPlayerCount; }

	void init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY) override;

	void update(float _fDeltaTime) override;
};

#endif //__PLAYER_H__