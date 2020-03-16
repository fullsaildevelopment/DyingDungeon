#ifdef __cplusplus
#define cbuffer struct
#define matrix DirectX::XMFLOAT4X4
#define float4 DirectX::XMFLOAT4
#define float3 DirectX::XMFLOAT3
#define uint UINT
#endif
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct Light
{
	float4 worldPosition;
	//------------------------------- ( 16 bytes )
	float4 worldDirection;
	//------------------------------- ( 16 bytes )
	float4 color;
	//------------------------------- ( 16 bytes )
	uint lightType;
	float intensity;
	float range;
	float cone;
	//------------------------------- ( 16 bytes )
}; //------------------------------- ( 4 x 16 bytes  = 64 bytes)

struct MaterialProperties
{
	float4 globalAmbient;
	//------------------------------- ( 16 bytes )
	float4 diffuseColor;
	//------------------------------- ( 16 bytes )
	float4 specularColor;
	//------------------------------- ( 16 bytes )
	float4 emissiveColor;
	//------------------------------- ( 16 bytes )
	float specularPower;
	float reflectance;
	uint hasDiffuseTexture;
	uint hasSpecularTexture;
	//------------------------------- ( 16 bytes )
	uint hasEmissiveTexture;
	uint hasNormalTexture;
	uint receiveShadows;
	float pad;
}; //------------------------------- ( 16 * 5 = 80 bytes )

cbuffer MaterialBuffer : register(b0)
{
	MaterialProperties mat;
};

cbuffer Lighting : register(b1)
{
	Light sceneLights[16];
	//------------------------------- ( 16 x 64 bytes = 1600 bytes)
	uint numLights;
	float3 camPos;
	//------------------------------- ( 16 bytes )
};  //------------------------------- ( 26 x 16 bytes = 1616 bytes)

cbuffer ShadowBuffer : register(b2)
{
	float4 shadowsEnabled;
};

Texture2D txDiffuse : register(t0);
Texture2D txEmissive : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txNormal : register(t3);
TextureCube txSkybox : register(t7);
Texture2D txStaticShadow : register(t8);
Texture2D txDynamicShadow : register(t9);

SamplerState samLinear : register(s0);
SamplerComparisonState samShadow : register(s1);