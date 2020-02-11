#include "PixelShaderIncludes.hlsli"

struct GSOutput
{
	float4 color : COLOR;
	float4 position : SV_Position;
	float2 tex : TEXCOORD;
	bool active : ACTIVE;
};

float4 main(GSOutput input) : SV_TARGET
{
	if (input.active == false)
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	clip(input.color.a - 0.005f);
	float4 color = txDiffuse.Sample(samLinear, input.tex);
	color.rgb *= input.color;
	return saturate(color);
}