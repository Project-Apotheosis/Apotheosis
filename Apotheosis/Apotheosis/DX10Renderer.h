// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : DX10Renderer.h
// Description :  DX10Renderer h
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#ifndef __DX10_RENDERER_H__
#define __DX10_RENDERER_H__

#include "Util.h"
#include "Camera.h"
#include "Vertex.h"
#include "SingleInstanceMap.h"
#include "Light.h"
#include <utility>
#include "RenderTasks.h"

	class DX10Renderer 
	{
	private: //Shadow

		
	private: //Construction

		DX10Renderer();
		static DX10Renderer* s_pInstance;
	public:

		~DX10Renderer();

		//Singleton 	
		static DX10Renderer* getInstance()
		{
			if (!s_pInstance)
			{
				s_pInstance = new DX10Renderer();
			}

			return s_pInstance;
		}

		//Init / shutdown functions to be called by IRoot Object before and after the game loop
		bool init(HWND, D3DInitializer*);
		void shutDown();

		//No copying
		DX10Renderer(const DX10Renderer&) = delete;
		DX10Renderer& operator= (const DX10Renderer&) = delete;

	private: //Data

		Camera		m_camera;

		SingleInstanceMap<ID3D10ShaderResourceView> m_textureMap;
		SingleInstanceMap<ID3D10Effect>				m_fxMap;
		SingleInstanceMap<ID3D10InputLayout>		m_inputLayoutMap;
		vector<RenderTask2D*>						m_2DRenderTasks;

		unordered_map<E_LIGHT_TYPE, vector<Light>>	m_lightMap{}; //All lights contained here, indexed by type and given IDs into vector

		char					m_cVideoCardDescription[128]{};
		int						m_iVideoCardMemory{};
		D3DInitializer*			m_pInitializer{}; //Stored on IRoot stack

		ID3D10Device*			m_pd3dDevice{};
		IDXGISwapChain*			m_pSwapChain{};
		ID3D10Texture2D*		m_pDepthStencilBuffer{};
		ID3D10RenderTargetView* m_pRenderTargetView{};
		ID3D10DepthStencilView* m_pDepthStencilView{};
		ID3D10RasterizerState*  m_pRasterizerState{};
		ID3DX10Font* m_pFont{};


	public: //Interface for IRoot

		void update(float _fDeltaTime);

		//Called by Win32App because it is a friend of IRoot which contains this renderer
		void onResize();

	public: //Statics as interface for GameRoot and Meshes
		static Camera* getCamera() { return &s_pInstance->m_camera; }
		static Camera* createCamera();


		static pair<E_LIGHT_TYPE, unsigned> createLight(E_LIGHT_TYPE _eType);
		static Light&	getLight(E_LIGHT_TYPE _eType, unsigned _iLightID);

		

		static void loadRenderTask2D(RenderTask2D& _taskToLoad, const string& _rTextureName, const string& _rkFXName, const string& _rkTechName, float _fDimX, float _fDimY);
		static void createTexture(const string& _rkTextureFileName, ID3D10ShaderResourceView** _ppRV);
	private: //Internals
		
		//GameRoot should use this to create lighting.fx
		void createEffectFromFile(const string& _krFXFileName);
		//Particles use this with empty mesh name as all particles use the same input layout
		void initializeShaders(const string& _krFXFileName, const string& _rkTechName, RenderTaskShaded* _pRenderTask, const string& _rkMeshName = "");


		void createEffectForRenderTask(const string& _krFXFileName, RenderTaskShaded* _pRenderTask);
		pair<const D3D10_INPUT_ELEMENT_DESC*, int> createVertexInputLayout(const string& _rkMeshName, RenderTaskShaded* _pRenderTask);

		void renderBegin(float _fDeltaTime);
		void renderMeshes(float _fDeltaTime);
		void renderEnd();

		//Get refresh rate, graphics card name and memory size 
		void grabAdapterDetails(int* _riNumerator = nullptr, int* _riDenominator = nullptr);



	};


#endif //__DX10_RENDERER_H__