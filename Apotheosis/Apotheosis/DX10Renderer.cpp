// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : DX10Renderer.cpp
// Description :  DX10Renderer cpp
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#include <dxgidebug.h>
#include <d3d11.h>
#include "DX10Renderer.h"
#include <dxgi.h>



	DX10Renderer* DX10Renderer::s_pInstance = nullptr; ///Singleton pointer

	/**
	*	Constructor
	*	@author Serge Radinovich
	*/
	DX10Renderer::DX10Renderer()
	{
	}

	/**
	*	Destructor
	*	@author Serge Radinovich
	*/
	DX10Renderer::~DX10Renderer()
	{
	}

	/**
	*	Initialize the renderer using application window handle and screen size data
	*	@author Serge Radinovich
	*	@param  _hWND				Handle to application window
	*	@param  _pInitializer		Data relating to screen size etc. Held by GameRoot
	*	@return bool				True if successful, false if failed
	*/
	bool DX10Renderer::init(HWND _hWND, D3DInitializer* _pInitializer)
	{
		//Device
		m_pInitializer = _pInitializer;


		int _iNumerator = 59;
		int _iDenominator = 1;
		//Get adapter name and memory. Also get Refresh rate details if in full screen mode
		if (m_pInitializer->bIsWindowed)
			grabAdapterDetails();
		else
			grabAdapterDetails(&_iNumerator, &_iDenominator);

		// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
		DXGI_SWAP_CHAIN_DESC _sd;
		_sd.BufferDesc.Width = m_pInitializer->iWidth;
		_sd.BufferDesc.Height = m_pInitializer->iHeight;
		_sd.Windowed = m_pInitializer->bIsWindowed;
		_sd.BufferDesc.RefreshRate.Numerator = _iNumerator;
		_sd.BufferDesc.RefreshRate.Denominator = _iDenominator;
		_sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		_sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// No multisampling.
		_sd.SampleDesc.Count = 1;
		_sd.SampleDesc.Quality = 0;

		_sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_sd.BufferCount = 1;
		_sd.OutputWindow = _hWND;

		_sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		_sd.Flags = 0;


		// Create the device.

		UINT _createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		_createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

		HR(D3D10CreateDeviceAndSwapChain(
			0,                 //default adapter
			D3D10_DRIVER_TYPE_HARDWARE,
			0,                 // no software device
			_createDeviceFlags,
			D3D10_SDK_VERSION,
			&_sd,
			&m_pSwapChain,
			&m_pd3dDevice));


		// The remaining steps that need to be carried out for d3d creation
		// also need to be executed every time the window is resized.  So
		// just call the onResize method here to avoid code duplication.

		onResize();


		//Font
		D3DX10_FONT_DESC _fontDesc;
		_fontDesc.Height = 24;
		_fontDesc.Width = 0;
		_fontDesc.Weight = 0;
		_fontDesc.MipLevels = 1;
		_fontDesc.Italic = false;
		_fontDesc.CharSet = DEFAULT_CHARSET;
		_fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
		_fontDesc.Quality = DEFAULT_QUALITY;
		_fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		wcscpy_s(_fontDesc.FaceName, L"Calibri");

		D3DX10CreateFontIndirect(m_pd3dDevice, &_fontDesc, &m_pFont);


		//set rasterizer	
		D3D10_RASTERIZER_DESC _rasterizerState;
		_rasterizerState.CullMode = D3D10_CULL_BACK;
		_rasterizerState.FillMode = D3D10_FILL_SOLID;
		_rasterizerState.FrontCounterClockwise = false;
		_rasterizerState.DepthBias = false;
		_rasterizerState.DepthBiasClamp = 0;
		_rasterizerState.SlopeScaledDepthBias = 0;
		_rasterizerState.DepthClipEnable = true;
		_rasterizerState.ScissorEnable = false;
		_rasterizerState.MultisampleEnable = false;
		_rasterizerState.AntialiasedLineEnable = true;


		m_pd3dDevice->CreateRasterizerState(&_rasterizerState, &m_pRasterizerState);
		m_pd3dDevice->RSSetState(m_pRasterizerState);


	
		return true;

	}


	void DX10Renderer::createTextureArray(const string& _rkTextureFileName, const string& _rkSuffix, const UINT _kiSpriteCount, ID3D10ShaderResourceView** _ppRV)
	{

		// Has this texture already been created?
		if (m_textureMap.containsItem(_rkTextureFileName))
		{
			*_ppRV = m_textureMap.getItem(_rkTextureFileName); //Grab existing texture
			return;
		}


		// Load the texture elements individually from file.  These textures
		// won't be used by the GPU (0 bind flags), they are just used to 
		// load the image data from file.  We use the STAGING usage so the
		// CPU can read the resource.
		unsigned _iArraySize = _kiSpriteCount;

		vector<ID3D10Texture2D*> _sourceTextures(_iArraySize, 0);

		for (unsigned i = 0; i < _iArraySize; ++i)
		{
			D3DX10_IMAGE_LOAD_INFO _loadInfo;

			_loadInfo.Width = D3DX10_FROM_FILE;
			_loadInfo.Height = D3DX10_FROM_FILE;
			_loadInfo.Depth = D3DX10_FROM_FILE;
			_loadInfo.FirstMipLevel = 0;
			_loadInfo.MipLevels = D3DX10_FROM_FILE;
			_loadInfo.Usage = D3D10_USAGE_STAGING;
			_loadInfo.BindFlags = 0;
			_loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
			_loadInfo.MiscFlags = 0;
			_loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			_loadInfo.Filter = D3DX10_FILTER_NONE;
			_loadInfo.MipFilter = D3DX10_FILTER_NONE;
			_loadInfo.pSrcInfo = 0;
			stringstream _ss; _ss << _rkTextureFileName << i << _rkSuffix; 
			string _sFullFilePath = resourceDirectory().append(_ss.str());
			HR(D3DX10CreateTextureFromFileA(m_pd3dDevice, _sFullFilePath.c_str(), &_loadInfo, 0, (ID3D10Resource**)&_sourceTextures[i], 0));
		}

		// Create the texture array.  Each element in the texture 
		// array has the same format/dimensions.
		D3D10_TEXTURE2D_DESC _texElementDesc;
		_sourceTextures[0]->GetDesc(&_texElementDesc);


		D3D10_TEXTURE2D_DESC _textureArrayDesc;
		_textureArrayDesc.Width = _texElementDesc.Width;
		_textureArrayDesc.Height = _texElementDesc.Height;
		_textureArrayDesc.MipLevels = _texElementDesc.MipLevels;
		_textureArrayDesc.ArraySize = _iArraySize;
		_textureArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_textureArrayDesc.SampleDesc.Count = 1;
		_textureArrayDesc.SampleDesc.Quality = 0;
		_textureArrayDesc.Usage = D3D10_USAGE_DEFAULT;
		_textureArrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		_textureArrayDesc.CPUAccessFlags = 0;
		_textureArrayDesc.MiscFlags = 0;

		ID3D10Texture2D* _pTexArray = 0;
		HR(m_pd3dDevice->CreateTexture2D(&_textureArrayDesc, 0, &_pTexArray));

		// Copy individual texture elements into texture array.
		// for each texture element...
		for (unsigned i = 0; i < _iArraySize; ++i)
		{
			// for each mipmap level...
			for (UINT j = 0; j < _texElementDesc.MipLevels; ++j)
			{
				D3D10_MAPPED_TEXTURE2D _mappedTex2D;
				_sourceTextures[i]->Map(j, D3D10_MAP_READ, 0, &_mappedTex2D);

				m_pd3dDevice->UpdateSubresource(_pTexArray, D3D10CalcSubresource(j, i, _texElementDesc.MipLevels),
					0, _mappedTex2D.pData, _mappedTex2D.RowPitch, 0);

				_sourceTextures[i]->Unmap(j);
			}
		}

		// Create a resource view to the texture array.
		D3D10_SHADER_RESOURCE_VIEW_DESC _viewDesc;
		_viewDesc.Format = _textureArrayDesc.Format;
		_viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
		_viewDesc.Texture2DArray.MostDetailedMip = 0;
		_viewDesc.Texture2DArray.MipLevels = _textureArrayDesc.MipLevels;
		_viewDesc.Texture2DArray.FirstArraySlice = 0;
		_viewDesc.Texture2DArray.ArraySize = _iArraySize;

		HR(m_pd3dDevice->CreateShaderResourceView(_pTexArray, &_viewDesc, _ppRV));

		// Cleanup--we only need the resource view.
		SAFE_RELEASE(_pTexArray);
		for (unsigned i = 0; i < _iArraySize; ++i)
			SAFE_RELEASE(_sourceTextures[i]);

		m_textureMap.addItem(_rkTextureFileName, *_ppRV);
	}




	/**
	*	If texture not previously loaded, load and save it in container
	*	@author Serge Radinovich
	*	@param  _rkTextureFileName	Texture file name
	*	@param  _ppRV				Out pointer to pointer to Resource View of the texture
	*/
	void DX10Renderer::createTexture(const string& _rkTextureFileName, ID3D10ShaderResourceView** _ppRV)
	{
		//Do not create duplicate textures
		if (s_pInstance->m_textureMap.containsItem(_rkTextureFileName))
		{
			*_ppRV = s_pInstance->m_textureMap.getItem(_rkTextureFileName); //Grab existing texture
		}
		else
		{
			//Create the diffuse texture
			string _sFullFilePath = resourceDirectory().append(_rkTextureFileName);
			HR(D3DX10CreateShaderResourceViewFromFileA(s_pInstance->m_pd3dDevice, _sFullFilePath.c_str(), 0, 0, _ppRV, 0));
			//Add newly made texture to manager
			s_pInstance->m_textureMap.addItem(_rkTextureFileName, *_ppRV);
		}
	}
	

	/**
	*	Get correct refresh rate based on screen size and find name of graphics card and memory size
	*	@author Serge Radinovich
	*	@param  _piNumerator		Return refresh rate numerator
	*	@param _piDenominator		Return refresh rate denominator
	*/
	void DX10Renderer::grabAdapterDetails(int* _piNumerator, int* _piDenominator)
	{
		IDXGIFactory* _pFactory;
		IDXGIAdapter* _pAdapter;
		IDXGIOutput* _pAdapterOutput;
		unsigned int _iNumModes, _iStringLength;
		DXGI_MODE_DESC* _pDisplayModeList;
		// Create a DirectX graphics interface factory.
		HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_pFactory));

		// Use the factory to create an adapter for the primary graphics interface (video card).
		HR(_pFactory->EnumAdapters(0, &_pAdapter));


		// Enumerate the primary adapter output (monitor).
		HR(_pAdapter->EnumOutputs(0, &_pAdapterOutput));


		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		HR(_pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &_iNumModes, NULL));


		// Create a list to hold all the possible display modes for this monitor/video card combination.
		_pDisplayModeList = new DXGI_MODE_DESC[_iNumModes];


		// Now fill the display mode list structures.
		HR(_pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &_iNumModes, _pDisplayModeList));


		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		if (_piNumerator && _piDenominator)
		{
			for (UINT i = 0; i < _iNumModes; i++)
			{
				if (_pDisplayModeList[i].Width == SCREEN_WIDTH_MAX)
				{
					if (_pDisplayModeList[i].Height == SCREEN_HEIGHT_MAX)
					{
						*_piNumerator = _pDisplayModeList[i].RefreshRate.Numerator;
						*_piDenominator = _pDisplayModeList[i].RefreshRate.Denominator;
						break;
					}
				}
			}
		}


		DXGI_ADAPTER_DESC _adapterDesc;

		// Get the adapter (video card) description.
		HR(_pAdapter->GetDesc(&_adapterDesc));


		// Store the dedicated video card memory in megabytes.
		m_iVideoCardMemory = (int)(_adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		wcstombs_s(&_iStringLength, m_cVideoCardDescription, 128, _adapterDesc.Description, 128);

		// Release the display mode list.
		SAFE_DELETE_ARRAY(_pDisplayModeList);
		// Release the adapter output.
		SAFE_RELEASE(_pAdapterOutput);
		// Release the adapter.
		SAFE_RELEASE(_pAdapter);
		// Release the factory.
		SAFE_RELEASE(_pFactory);
	}

	/**
	*	Shut down singleton
	*	@author Serge Radinovich
	*/
	void DX10Renderer::shutDown()
	{

		m_textureMap.shutDownRelease();
		m_fxMap.shutDownRelease();
		m_inputLayoutMap.shutDownRelease();

		for (auto& pRenderTask2D : m_2DRenderTasks)
			SAFE_RELEASE(pRenderTask2D->pMesh);


		m_pd3dDevice->ClearState();

		SAFE_RELEASE(m_pRasterizerState);
		SAFE_RELEASE(m_pSwapChain);
		SAFE_RELEASE(m_pDepthStencilBuffer);
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pDepthStencilView);

		SAFE_RELEASE(m_pFont);


		SAFE_RELEASE(m_pd3dDevice);
		SAFE_DELETE(s_pInstance);

	}

	/**
	*	Called from AppWin32 message pump whenever size and resize messages are processed
	*	@author Serge Radinovich
	*/
	void DX10Renderer::onResize()
	{
		if (!m_pInitializer || !m_pSwapChain) //Cannot resize if renderer does not have details from Root as to screen size
			return;

		// Release the old views, as they hold references to the buffers we
		// will be destroying.  Also release the old depth/stencil buffer.

		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pDepthStencilView);
		SAFE_RELEASE(m_pDepthStencilBuffer);


		// Resize the swap chain and recreate the render target view.
		//Height and width were updated from Win32App window procedure
		HR(m_pSwapChain->ResizeBuffers(1, m_pInitializer->iWidth, m_pInitializer->iHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		ID3D10Texture2D* _pBackBuffer;
		HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&_pBackBuffer)));
		HR(m_pd3dDevice->CreateRenderTargetView(_pBackBuffer, 0, &m_pRenderTargetView));
		SAFE_RELEASE(_pBackBuffer);


		// Create the depth/stencil buffer and view.

		D3D10_TEXTURE2D_DESC _depthStencilDesc;

		_depthStencilDesc.Width = m_pInitializer->iWidth;
		_depthStencilDesc.Height = m_pInitializer->iHeight;
		_depthStencilDesc.MipLevels = 1;
		_depthStencilDesc.ArraySize = 1;
		_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_depthStencilDesc.SampleDesc.Count = 1; // multisampling must match
		_depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
		_depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
		_depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		_depthStencilDesc.CPUAccessFlags = 0;
		_depthStencilDesc.MiscFlags = 0;

		HR(m_pd3dDevice->CreateTexture2D(&_depthStencilDesc, 0, &m_pDepthStencilBuffer));
		HR(m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));


		// Bind the render target view and depth/stencil view to the pipeline.

		m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


		// Set the viewport transform.
		D3D10_VIEWPORT _vp;
		_vp.TopLeftX = 0;
		_vp.TopLeftY = 0;
		_vp.Width = m_pInitializer->iWidth;
		_vp.Height = m_pInitializer->iHeight;
		_vp.MinDepth = 0.0f;
		_vp.MaxDepth = 1.0f;

		m_pd3dDevice->RSSetViewports(1, &_vp);
	}

	/**
	*	Create a new light
	*	@author Serge Radinovich
	*	@param	_eType							Enum as to type of light to create
	*	@return pair<E_LIGHT_TYPE, unsigned>	Handle to newly created light
	*/
	pair<E_LIGHT_TYPE, unsigned> DX10Renderer::createLight(E_LIGHT_TYPE _eType)
	{
		//Get next ID for object
		unsigned _iReturnID = s_pInstance->m_lightMap[_eType].size();

		//Create the new light
		s_pInstance->m_lightMap[_eType].push_back(Light());
		//Give the light required properties based on type
		Light::generateLightType(s_pInstance->m_lightMap[_eType].back(), _eType);

		return{ _eType, _iReturnID };
	}

	/**
	*	Get a light that user created
	*	@author Serge Radinovich
	*	@param	_eType							Enum as to type of light to create
	*	@param	_iLightID						Index into light type vector that represents the requested light
	*	@return Light&							Reference to the requested light
	*/
	Light&	DX10Renderer::getLight(E_LIGHT_TYPE _eType, unsigned _iLightID)
	{
		assert(!s_pInstance->m_lightMap[_eType].empty() && "Tried to get a light from an empty vector of lights");
		assert(s_pInstance->m_lightMap[_eType].size() > _iLightID && "Entered light ID bigger than or equal to size of light vector");

		return s_pInstance->m_lightMap[_eType][_iLightID];

	}


	/**
	*	Update loop of the renderer
	*	@author Serge Radinovich
	*	@param	_fDeltaTime						DT
	*/
	void DX10Renderer::update(float _fDeltaTime)
	{
		renderBegin(_fDeltaTime);
		renderEnd();
	}


	/**
	*	Initiate a render pass for this frame
	*	@author Serge Radinovich
	*	@param	_fDeltaTime						DT
	*/
	void DX10Renderer::renderBegin(float _fDeltaTime)
	{
		m_pd3dDevice->ClearRenderTargetView(m_pRenderTargetView, LIGHT_GREY);
		m_pd3dDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

		renderMeshes(_fDeltaTime);

	}


	/**
	*	Render all meshes this frame
	*	@author Serge Radinovich
	*	@param	_fDeltaTime						DT
	*/
	void DX10Renderer::renderMeshes(float _fDeltaTime)
	{
		if (m_2DRenderTasks.empty())
			return;


		const RenderTask2D* _kpRenderTask{};

		for (UINT iTaskIndex = 0; iTaskIndex < m_2DRenderTasks.size(); ++iTaskIndex)
		{
			_kpRenderTask = m_2DRenderTasks[iTaskIndex];

			//Next task if not rendering this one
			if (!_kpRenderTask->rendering)
				continue;

			static float s_fGameTime = 0.0f;
			s_fGameTime += _fDeltaTime;
			if (_kpRenderTask->pFXGameTimeVar)
				_kpRenderTask->pFXGameTimeVar->SetFloat(s_fGameTime);

			_kpRenderTask->pFX->GetVariableByName("gLight")->SetRawValue(&m_lightMap[LT_PARALLEL].back(), 0, sizeof(Light));
			_kpRenderTask->pFXEyePosWVar->SetRawValue(&m_camera.m_transform.translation, 0, sizeof(D3DXVECTOR3));


			_kpRenderTask->pFXTexArrayVar->AsShaderResource()->SetResource(_kpRenderTask->pTextureArray);
			_kpRenderTask->pFXAnimRateVar->AsScalar()->SetFloat(_kpRenderTask->fAnimRate);
			_kpRenderTask->pFXSpriteCountVar->AsScalar()->SetFloat(_kpRenderTask->iSpriteCount);

			//if (_kpRenderTask->textures[T_DIFFUSE])
			//{
			//	_kpRenderTask->pFXDiffuseMapVar->AsShaderResource()->SetResource(_kpRenderTask->textures[T_DIFFUSE]);
			//	_kpRenderTask->pFXSpecMapVar->AsShaderResource()->SetResource(_kpRenderTask->textures[T_SPECULAR]);
			//	_kpRenderTask->pFXEyePosWVar->SetRawValue(&m_camera.m_transform.translation, 0, sizeof(D3DXVECTOR3));

			//	_kpRenderTask->pFX->GetVariableByName("gLight")->SetRawValue(&m_lightMap[LT_PARALLEL].back(), 0, sizeof(Light)); 
			//	//				//int i = 1;
			//	//				//_kpRenderTask->pFX->GetVariableByName("gLightType")->SetRawValue(&i, 0, sizeof(int));
			//	//				_kpRenderTask->pFXTexMtxVar->AsMatrix()->SetMatrix((float*)&_kpRenderTask->textureMatrix);
			//}

			//Set up transformation matrix for shader		
			_kpRenderTask->pFXWVPVar->AsMatrix()->SetMatrix((float*)(_kpRenderTask->transformMtx * m_camera.m_viewMtx * m_camera.m_projMtx));
			_kpRenderTask->pFXWorldVar->AsMatrix()->SetMatrix((float*)D3DXMATRIX(_kpRenderTask->transformMtx));

			_kpRenderTask->pFX->GetVariableByName("gCurrentTextureIndex")->AsScalar()->SetInt(_kpRenderTask->iTextureIndex);

			//Set the input layout for vertices
			m_pd3dDevice->IASetInputLayout(_kpRenderTask->pInputLayout);

			//Make passes
			for (unsigned _iPass = 0; _iPass < _kpRenderTask->techDesc.Passes; _iPass++)
			{
				//apply technique
				_kpRenderTask->pTech->GetPassByIndex(_iPass)->Apply(0);
				//Draw
				HR(_kpRenderTask->pMesh->DrawSubset(0));
				
			}
		}

	}


	/**
	*	Present render results to screen
	*	@author Serge Radinovich
	*/
	void DX10Renderer::renderEnd()
	{

		if (!m_pInitializer->bIsWindowed && m_pInitializer->bVSYNC) //Fullscreen with vsync
		{
			HR(m_pSwapChain->Present(1, 0));
		}
		else
		{
			HR(m_pSwapChain->Present(0, 0));
		}

	}


	/**
	*	Create the camera for the game
	*	@author Serge Radinovich
	*	@return	Camera*			Pointer to the camera
	*/
	Camera* DX10Renderer::createCamera()
	{
		float _fAspect = static_cast<float>(s_pInstance->m_pInitializer->iWidth / s_pInstance->m_pInitializer->iHeight);

		s_pInstance->m_camera.init(_fAspect);
		D3DXVECTOR3 _pos(0.0f, 10.0f, -10.0f);
		D3DXVECTOR3 _target(0.0f, 0.0f, 0.0f);
		s_pInstance->m_camera.setPos(&_pos);
		s_pInstance->m_camera.lookAt(&_target);

		return &s_pInstance->m_camera;
	}

	/**
	*	Load an FX file data into an FX pointer in the supplied render task
	*	@author Serge Radinovich
	*	@param	_krFXFileName				Name of the FX file
	*	@param	_pRenderTask				Render task into which the fx data is loaded
	*/
	void DX10Renderer::createEffectForRenderTask(const string& _krFXFileName, RenderTaskShaded* _pRenderTask)
	{
		//Do not attempt to create a duplicate of existing FX file
		if (m_fxMap.containsItem(_krFXFileName))
		{
			_pRenderTask->pFX = m_fxMap.getItem(_krFXFileName);
		}
		else
		{
			//Create the effect and put it into fxManager
			createEffectFromFile(_krFXFileName);
			//Give the render task the newly created FX
			_pRenderTask->pFX = m_fxMap.getItem(_krFXFileName);
		}
	}

	/**
	*	Load vertex input layout for a render task based on their mesh and shader
	*	This engine only has one vertex type at the moment
	*	@author Serge Radinovich
	*	@param	_rkMeshName									Name of the mesh
	*	@param	_pRenderTask								Render task into which the input layout data is loaded
	*	@return	pair<const D3D10_INPUT_ELEMENT_DESC*, int>	Data loaded in this function	
	*/
	pair<const D3D10_INPUT_ELEMENT_DESC*, int> DX10Renderer::createVertexInputLayout(const string& _rkMeshName, RenderTaskShaded* _pRenderTask)
	{
		string _sMeshType = _rkMeshName;

		//We only have one vertex type so dont actually need to set these 2 values at run-time
		int _iElementCount = 3;
		const D3D10_INPUT_ELEMENT_DESC* _pkInputDesc = g_kInputLayoutVertexPNT;

		//Create the layout or grab existing
		if (m_inputLayoutMap.containsItem(_sMeshType)) //Grab existing input layout
		{
			_pRenderTask->pInputLayout = m_inputLayoutMap.getItem(_sMeshType);
		}
		else //Make new input layout
		{
			D3D10_PASS_DESC _passDesc;
			HR(_pRenderTask->pTech->GetPassByIndex(0)->GetDesc(&_passDesc));
			HR(m_pd3dDevice->CreateInputLayout(_pkInputDesc, _iElementCount, _passDesc.pIAInputSignature, _passDesc.IAInputSignatureSize, &_pRenderTask->pInputLayout));
			//Add this layout to the manager and index it via the name of the mesh
			m_inputLayoutMap.addItem(_sMeshType, _pRenderTask->pInputLayout);
		}


		return{ _pkInputDesc, _iElementCount };
	}

	/**
	*	Initialize each game object's shaders
	*	@author Serge Radinovich
	*	@param	_krFXFileName							FX file name
	*	@param	_rkTechName								FX file technique name
	*	@param	_pRenderTask							Render task into which the data is loaded
	*	@param	_rkMeshName								Name of the mesh
	*	@param	pair<const D3D10_INPUT_ELEMENT_DESC*, int>	Data loaded in this function
	*/
	void DX10Renderer::initializeShaders(const string& _krFXFileName, const string& _rkTechName, RenderTaskShaded* _pRenderTask, const string& _rkMeshName)
	{
		//Create the FX object	
		createEffectForRenderTask(_krFXFileName, _pRenderTask);

		//Get technique from newly created FX 
		_pRenderTask->pTech = _pRenderTask->pFX->GetTechniqueByName(_rkTechName.c_str());
		//Get technique description
		_pRenderTask->pTech->GetDesc(&_pRenderTask->techDesc);

		//Create the Vertex Input layout required for rendering based on vertextype
		pair<const D3D10_INPUT_ELEMENT_DESC*, int> _inputElementDescPair = createVertexInputLayout(_rkMeshName, _pRenderTask);
		//Grab the details and add to render task
		_pRenderTask->pkInputDesc = _inputElementDescPair.first;
		_pRenderTask->iDescElementCount = _inputElementDescPair.second;

	}


	/**
	*	Load an FX file
	*	@author Serge Radinovich
	*	@param	_krFXFileName							FX file name
	*/
	void DX10Renderer::createEffectFromFile(const string& _krFXFileName)
	{
		assert(!m_fxMap.containsItem(_krFXFileName));


		DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compilationErrors = 0;
		HRESULT hr = 0;
		ID3D10Effect* _pNewFX{};
		hr = D3DX10CreateEffectFromFileA(_krFXFileName.c_str(), NULL, NULL, "fx_4_0", shaderFlags, 0, m_pd3dDevice, NULL, NULL, &_pNewFX, &compilationErrors, NULL);
		if (FAILED(hr))
		{
			if (compilationErrors)
			{
				MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
				SAFE_RELEASE(compilationErrors);
			}
			const WCHAR* errorString = DXGetErrorString(hr);
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
		}

		//Create the FX file
		//Give the manager a pointer to new fx file
		m_fxMap.addItem(_krFXFileName, _pNewFX);
	}

	/**
	*	Initialize each game object for rendering
	*	@author Serge Radinovich
	*	@param	_taskToLoad							Render task into which the data is loaded
	*	@param	_rTextureName						Name of the texture
	*	@param	_krFXFileName						FX file name
	*	@param	_rkTechName							FX file technique name	
	
	*	@param	pair<const D3D10_INPUT_ELEMENT_DESC*, int>	Data loaded in this function
	*/
	void DX10Renderer::loadRenderTask2D(RenderTask2D& _taskToLoad, const string& _rTextureName, const string& _rkSuffix, const UINT _kiSpriteCount, const string& _rkFXName, const string& _rkTechName, float _fDimX, float _fDimY)
	{
		//SHADERS
		s_pInstance->initializeShaders(_rkFXName, _rkTechName, &_taskToLoad, _rTextureName);

		//MESH
		//vertices 
		VertexPNT _pVertArray[4];
		// Create the index buffer
		DWORD _pIndexArray[6];
		//Initialize values of both

		// Fill in the quad
		_pVertArray[0] = VertexPNT(-_fDimX, 0.0f, _fDimY, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f); //Top left
		_pVertArray[1] = VertexPNT(_fDimX, 0.0f, _fDimY, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f); //Top right
		_pVertArray[2] = VertexPNT(-_fDimX, 0.0f, -_fDimY, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f); //Bottom left
		_pVertArray[3] = VertexPNT(_fDimX, 0.0f, -_fDimY, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f); //Bottom right

		_pIndexArray[0] = 0;
		_pIndexArray[1] = 1;
		_pIndexArray[2] = 2;
		_pIndexArray[3] = 1;
		_pIndexArray[4] = 3;
		_pIndexArray[5] = 2;

		//create mesh
		D3DX10CreateMesh(s_pInstance->m_pd3dDevice, _taskToLoad.pkInputDesc, _taskToLoad.iDescElementCount, "POSITION", 4, 2, D3DX10_MESH_32_BIT, &_taskToLoad.pMesh);

		//insert data into mesh and commit changes
		_taskToLoad.pMesh->SetVertexData(0, _pVertArray);
		_taskToLoad.pMesh->SetIndexData(_pIndexArray, 6);
		_taskToLoad.pMesh->CommitToDevice();

		s_pInstance->m_2DRenderTasks.push_back(&_taskToLoad);


		//TEXTURE
		//s_pInstance->createTexture(_rTextureName, &_taskToLoad.textures[0]);

		s_pInstance->createTextureArray(_rTextureName, _rkSuffix, _kiSpriteCount, &_taskToLoad.pTextureArray);
	}