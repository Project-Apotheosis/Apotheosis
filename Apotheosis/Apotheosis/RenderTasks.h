// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : RenderTasks.h
// Description :  RenderTasks h
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#ifndef __RENDER_TASKS_H__
#define __RENDER_TASKS_H__

#include <string>
#include <sstream>
#include <array>

#include <Windows.h>
#include "Util.h"

struct RenderTaskShaded
	{
		RenderTaskShaded()
		{
			ZeroMemory(this, sizeof(*this));
		}

		bool			rendering{ false };
		ID3D10InputLayout* pInputLayout{};

		//FX
		ID3D10Effect*		   pFX{};
		ID3D10EffectTechnique* pTech{};
		D3D10_TECHNIQUE_DESC   techDesc;
		//For creation
		const D3D10_INPUT_ELEMENT_DESC* pkInputDesc{};
		int								iDescElementCount{};
	};


	


	enum E_TEXTURE { T_DIFFUSE, T_SPECULAR, T_CUBE, T_NORMAL, T_4, T_5, T_MAX };
	enum E_SHADER_VAR_SCALAR { SVS_DELTATIME, SVS_CUBEMAP_ENABLED };
	enum E_SHADER_VAR_MATRIX { SVM_WVP, SVM_WORLD, SVM_LIGHTWVP, SVM_TEXMTX };
	enum E_SHADER_VAR_VECTOR { SVV_EYEPOSW, SVV_DIRTOSUN };
	enum E_SHADER_VAR_RESOURCE { SVR_DIFFUSEMAP, SVR_SPECMAP, SVR_CUBEMAP, SVR_NORMALMAP, SVR_LAYER0, SVR_LAYER1, SVR_LAYER2, SVR_LAYER3, SVR_LAYER4, SVR_BLENDMAP };

	struct RenderTask2D : public RenderTaskShaded
	{
		RenderTask2D()
		{
			ZeroMemory(this, sizeof(*this));
			D3DXMatrixIdentity(&transformMtx);
			D3DXMatrixIdentity(&textureMatrix);
		}

		//Mesh
		ID3DX10Mesh*	   pMesh{};
		//Custom mesh
		ID3D10Buffer*	   pVB{};
		ID3D10Buffer*	   pIB{};
		UINT			   iVertCount{};
		UINT			   iIndexCount{};
		UINT			   iStride{};

		//Animation
		UINT iSpriteCount{};
		float fAnimRate{};

		//Scalars
		ID3D10EffectScalarVariable* pFXDeltaTimeVar{};
		ID3D10EffectScalarVariable* pFXGameTimeVar{};
		ID3D10EffectScalarVariable* pFXAnimRateVar{};
		//ID3D10EffectScalarVariable* pFXLastAnimTimeVar{};
		ID3D10EffectScalarVariable* pFXSpriteCountVar{};
		//ID3D10EffectScalarVariable* pFXCubeMapEnabledVar{};


		//Matrix vars
		ID3D10EffectMatrixVariable* pFXWVPVar{};
		ID3D10EffectMatrixVariable* pFXWorldVar{};
		ID3D10EffectMatrixVariable* pFXLightWVPVar{};
		ID3D10EffectMatrixVariable* pFXTexMtxVar{};

		//Vector vars
		ID3D10EffectVectorVariable* pFXEyePosWVar{};

		//Resource vars
		ID3D10EffectShaderResourceVariable* pFXTexArrayVar{};


		//Textures
		UINT iTextureIndex{};
		ID3D10ShaderResourceView* pTextureArray{};
		std::array<ID3D10ShaderResourceView*, T_MAX> textures{};
		D3DXMATRIX				  textureMatrix;

		//Shadow
		bool			bShadows{ false };
		ID3D10EffectVectorVariable* pFXDirToSunWVar{};


		D3DXMATRIX		transformMtx;

		//UV inversion for left / right facing
		bool bDefaultUVs{ true };

	};


#endif //__RENDER_TASKS_H__