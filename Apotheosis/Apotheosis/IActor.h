// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : IActor.h
// Description :  IActor h
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#ifndef __ACTOR_H__
#define __ACTOR_H__


#include "Util.h"
#include "Transform.h"
#include "DX10Renderer.h"
#include "PhysicsSystem.h"
#include <queue>
	class IActor
	{
	public: //Construction

		IActor()
		{
			Transform::rotateAroundRight(&m_transform, -D3DX_PI / 2);
			m_renderTasks.reserve(10);
			m_textureAnimQueues.reserve(10);
		}

		virtual	~IActor()
		{
		}

		virtual void init(const b2Vec2& _rkPosition, float _fDimX, float _fDimY) = 0;

	protected: //Data

		Transform		m_transform;
		vector<RenderTask2D>	m_renderTasks;
		vector<b2Body*>			m_rigidBodies;
		vector<queue<ID3D10ShaderResourceView*>> m_textureAnimQueues{};
		float m_fAnimRate{ 1.0f };
		float m_fLastAnim{};

		UINT m_iCurrTaskIndex{ 0 };

	public: //Interface

		Transform& transform() { return m_transform; }
		vector<RenderTask2D>& renderTask() { return m_renderTasks; }
		vector<b2Body*>& rigidBodies() { return m_rigidBodies; }

		void initializeRendering(const string& _rkTexture, const string& _rkFX, const string& _rkTech, float _fDimX, float _fDimY)
		{
			m_renderTasks.push_back(RenderTask2D());
			m_textureAnimQueues.push_back(queue<ID3D10ShaderResourceView*>());

			DX10Renderer::loadRenderTask2D(m_renderTasks.back(), _rkTexture, _rkFX, _rkTech, _fDimX, _fDimY);
			m_renderTasks.back().rendering = true;

			//Grab all the shader variables so the render task can access them
			m_renderTasks.back().pFXEyePosWVar = m_renderTasks.back().pFX->GetVariableByName("gEyePosW")->AsVector();

			m_renderTasks.back().pFXWorldVar = m_renderTasks.back().pFX->GetVariableByName("gWorld")->AsMatrix();
			m_renderTasks.back().pFXWVPVar = m_renderTasks.back().pFX->GetVariableByName("gWVP")->AsMatrix();
			m_renderTasks.back().pFXTexMtxVar = m_renderTasks.back().pFX->GetVariableByName("gTexMtx")->AsMatrix();

			m_renderTasks.back().pFXDiffuseMapVar = m_renderTasks.back().pFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
			m_renderTasks.back().pFXSpecMapVar = m_renderTasks.back().pFX->GetVariableByName("gSpecMap")->AsShaderResource();

			m_renderTasks.back().transformMtx = *m_transform.matrix();

			m_textureAnimQueues.back().push(m_renderTasks.back().textures[0]);
		}

		void addTexture(const string& _rkTexture)
		{
			//Iterate through all possible texture slots on render task
			for (UINT i = 0; i < T_MAX; i++)
			{
				//Find empty slot
				if (m_renderTasks.back().textures[i] == nullptr)
				{
					//Load the texture
					DX10Renderer::createTexture(_rkTexture, &m_renderTasks.back().textures[i]);
					//Add the texture to the animation queue
					m_textureAnimQueues.back().push(m_renderTasks.back().textures[i]);
					return;
				}

			}
			assert(false && "Too many textures added to this IActor.");
		}

		/**
		*	Update every frame. Use this function to manipulate components
		*	@param _fDeltaTime	Change in time since last frame
		*/
		virtual void update(float _fDeltaTime) = 0;

	protected:

		void setCurrState(UINT _iIndex)
		{
			assert(_iIndex >= 0 && _iIndex < m_renderTasks.size() && "Invalid index number given for setting IActor state.");
			
			m_iCurrTaskIndex = _iIndex;
			for (UINT i = 0; i < m_renderTasks.size(); ++i)
			{
				if (i == m_iCurrTaskIndex)
					m_renderTasks[i].rendering = true;
				else
					m_renderTasks[i].rendering = false;
			}
		}

		void animate(float _fDeltaTime)
		{
			m_fLastAnim += _fDeltaTime;
			if (m_fLastAnim > m_fAnimRate)
			{
				m_fLastAnim = 0.0f;
				ID3D10ShaderResourceView* _pTemp = m_textureAnimQueues[m_iCurrTaskIndex].front();
				m_textureAnimQueues[m_iCurrTaskIndex].pop();
				m_textureAnimQueues[m_iCurrTaskIndex].push(_pTemp);
				m_renderTasks[m_iCurrTaskIndex].textures[T_DIFFUSE] = m_textureAnimQueues[m_iCurrTaskIndex].front();
			}
			
		}

	};



#endif //__ACTOR_H__

