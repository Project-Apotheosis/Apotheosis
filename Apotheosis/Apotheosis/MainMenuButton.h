#ifndef __MAIN_MENU_BUTTON_H__
#define __MAIN_MENU_BUTTON_H__

#include "IButton.h"
#include "IActor.h"

class MainMenuButton : public IButton, public IActor
{
public: //Construction

	MainMenuButton(){}
	~MainMenuButton(){}

private: //Data

	float m_fDimX{};
	float m_fDimY{};

public: //Interface

	//IButton Overrides*********************


	void highlight(bool highlighted) override
	{
		m_iCurrentTextureIndex = highlighted ? 1 : 0;
	}

	void click() override
	{
		IButton::click();
	}
	//IButton Overrides////////////////////

	//IActor Overrides*********************
	void init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY) override
	{
		m_transform.translation.x = _rkPosition.x;
		m_transform.translation.y = _rkPosition.y;

		m_fDimX = _fDimX;
		m_fDimY = _fDimY;
	}

	void addRenderable(const string& _rkTextureName, const string& _rkSuffix, const UINT _kiSpriteCount)
	{
		//Rendering
		IActor::initializeRendering(_rkTextureName, _rkSuffix, _kiSpriteCount, "button.fx", "ButtonTech", m_fDimX, m_fDimY);
	}

	void update(float _fDeltaTime) override
	{
		m_renderTasks[0].iTextureIndex = m_iCurrentTextureIndex;
	}
	//IActor Overrides/////////////////////
};

#endif //__MAIN_MENU_BUTTON_H__