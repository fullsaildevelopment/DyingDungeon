#include "PixelShaderIncludes.hlsli"

struct PIXEL_SHADER_INPUT
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

float4 main(PIXEL_SHADER_INPUT input) : SV_Target
{
	return txSkybox.Sample(samLinear, input.texCoord.xyz);
}