#include "VertexShaderIncludes.hlsli"

struct VSIn
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct VSOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VSOut main(VSIn input)
{
	VSOut output;
	float4 position = float4(input.position.xyz, 1.0f);
	float4 color = float4(input.color.xyz, 1.0f);
	position = mul(view, position);
	position = mul(proj, position);
	output.position = position;
	output.color = color;
	return output;
}