#include "PixelShaderIncludes.hlsli"

struct GSOutput
{
	float3 origin : ORIGIN;
	float4 position : SV_Position;
	float3 velocity : VELOCITY;
    float3 color : COLOR;
    float2 tex : TEXCOORD;
	float lifeTime : LIFETIME;
};

float4 main(GSOutput input) : SV_TARGET
{
    float4 color = txDiffuse.Sample(samLinear, input.tex);
    color.rgb *= input.color;
    return color;
}