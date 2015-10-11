struct Light
{
	float3 pos;
	float3 dir;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att;
	float  spotPower;
	float  range;
};

struct SurfaceInfo
{
	float3 pos;
	float3 normal;
	float4 diffuse;
	float4 spec;
};

bool gDefaultUVs;

float3 ParallelLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);



		// The light vector aims opposite the direction the light rays travel.
		float3 lightVec = -L.dir;

		// Add the ambient term.
		litColor += v.diffuse * L.ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if (diffuseFactor > 0.0f)
	{
		float specPower = max(v.spec.a, 1.0f);
		float3 toEye = normalize(eyePos - v.pos);
			float3 R = reflect(-lightVec, v.normal);
			float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}

	return litColor;
}

BlendState gBlend
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

cbuffer cbPerFrame
{
	Light gLight;
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP; 
	float4x4 gTexMtx;
	float gAnimRate;
	int gSpriteCount;
	float gGameTime;
};

// Nonnumeric values cannot be added to a cbuffer.
//Texture2D gDiffuseMap;
//Texture2D gSpecMap;
Texture2DArray gTexArray;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float3 posL    : POSITION;
	float3 normalL : NORMAL;
	float2 texC    : TEXCOORD;
};

struct VS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
	float2 texC    : TEXCOORD;
    //float2 texC0   : TEXCOORD0;
    //float2 texC1   : TEXCOORD1;
};
 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Transform to world space space.
	vOut.posW    = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);
	
	// Output vertex attributes for interpolation across triangle.
	//vOut.texC0  = vIn.texC;
	//vOut.texC1  = mul(float4(vIn.texC, 0.0f, 1.0f), gTexMtx);
	vOut.texC = vIn.texC;
	if (!gDefaultUVs)
	{
		vOut.texC.x = 1.0f - vOut.texC.x;
	}
	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	// Get materials from texture maps.
	//float alpha    = gDiffuseMap.Sample( gTriLinearSam, pIn.texC0 ).a;
	//float4 diffuse = gDiffuseMap.Sample( gTriLinearSam, pIn.texC1 );
	//float alpha    = gDiffuseMap.Sample( gTriLinearSam, pIn.texC ).a;
	//float4 diffuse = gDiffuseMap.Sample(gTriLinearSam, pIn.texC);
	//float4 spec    = gSpecMap.Sample( gTriLinearSam, pIn.texC );
	
	// Interpolating normal can make it not be of unit length so normalize it.
    float3 normalW = normalize(pIn.normalW);
    
	int index = (gGameTime * gAnimRate) % gSpriteCount;
	float4 diffuse = gTexArray.Sample(gTriLinearSam, float3(pIn.texC, index));

		//float4 spec = diffuse;
		// Map [0,1] --> [0,256]
		//	spec.a *= 256.0f;


		// Compute the lit color for this pixel.
		//SurfaceInfo v = {pIn.posW, normalW, diffuse, spec};
		//float3 litColor = ParallelLight(v, gLight, gEyePosW);
		return diffuse;
    //return float4(litColor, 1.0f);
}

technique10 TexAlphaTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );

		//Use "blend" for this pass
		SetBlendState(gBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
    }
}
