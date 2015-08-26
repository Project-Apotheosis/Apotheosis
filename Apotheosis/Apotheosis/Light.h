#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Util.h"


	enum E_LIGHT_TYPE : int { LT_PARALLEL = 0, LT_POINT = 1, LT_SPOT = 2, LT_MAX };

	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(*this));
		}

		~Light()
		{
		}

		D3DXVECTOR3 pos;
		float		pad1;      // not used
		D3DXVECTOR3 dir;
		float		pad2;      // not used
		D3DXCOLOR	ambient;
		D3DXCOLOR	diffuse;
		D3DXCOLOR	specular;
		D3DXVECTOR3 att;
		float		spotPow;
		float		range;


		//Statics
		static void generateLightType(Light& _rLightToSet, E_LIGHT_TYPE _eType)
		{
			switch (_eType)
			{
			case LT_PARALLEL:
				//_rLightToSet.dir = D3DXVECTOR3(-0.57735f, -0.57735f, -0.57735f);
				//_rLightToSet.dir = D3DXVECTOR3(0.57735f, -0.57735f, -0.57735f);
				_rLightToSet.dir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				_rLightToSet.ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				_rLightToSet.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				_rLightToSet.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;

			case LT_POINT:
				_rLightToSet.ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
				_rLightToSet.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				_rLightToSet.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				_rLightToSet.att.x = 0.0f;
				_rLightToSet.att.y = 0.1f;
				_rLightToSet.att.z = 0.0f;
				_rLightToSet.range = 50.0f;
				break;

			case LT_SPOT:
				_rLightToSet.ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
				_rLightToSet.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				_rLightToSet.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				_rLightToSet.att.x = 1.0f;
				_rLightToSet.att.y = 0.0f;
				_rLightToSet.att.z = 0.0f;
				_rLightToSet.spotPow = 64.0f;
				_rLightToSet.range = 10000.0f;
				break;
			}
		}



	};



#endif //__LIGHT_H__