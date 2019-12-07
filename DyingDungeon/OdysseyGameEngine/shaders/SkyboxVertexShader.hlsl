#include "VertexShaderIncludes.hlsli"

struct VERTEX_SHADER_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 tex : TEXCOORD;
};

struct PIXEL_SHADER_INPUT
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

PIXEL_SHADER_INPUT main(VERTEX_SHADER_INPUT input)
{
	PIXEL_SHADER_INPUT output;
	float4 position = float4(input.position.xyz, 1.0f);
	output.position = mul(worldViewProj, position);
	output.texCoord = position.xyz;
	return output;
}