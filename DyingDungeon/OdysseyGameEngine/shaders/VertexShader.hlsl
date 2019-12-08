#include "VertexShaderIncludes.hlsli"

struct VERTEX_SHADER_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float4 jointWeight : BLENDWEIGHTS;
	int4 jointIndices : BLENDINDICES;
	float2 tex : TEXCOORD;
};

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 tex : TEXCOORD;
	float4 lightViewPosition : POSITION1;
};

cbuffer AnimationData : register(b1)
{
	float4 hasAnimationData;
	float4x4 finalPose[128];
};

PixelShaderInput main(VERTEX_SHADER_INPUT input)
{
	PixelShaderInput output;

	float4 position = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 normal = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (hasAnimationData.x == 1.0f)
	{
		for (int i = 0; i < 4; i++)
		{
			int index = input.jointIndices[i];
			float4 vPosition = float4(input.position.xyz, 1.0f);

			position += mul(vPosition, finalPose[index]) * input.jointWeight[i];
			normal += mul(float4(input.normal.xyz, 0.0f), finalPose[index]) * input.jointWeight[i];
		}
	}
	else
	{
		position = float4(input.position.xyz, 1.0f);
		normal = float4(input.normal.xyz, 0.0f);
	}

	float4 tangent = float4(input.tangent.xyz, 0.0f);
	float4 binormal = float4(input.binormal.xyz, 0.0f);

	output.position = mul(worldViewProj, position);
	output.worldPosition = mul(world, position).xyz;
	output.normal = mul(world, normal).xyz;
	output.normal = normalize(output.normal);
	output.tangent = mul(world, tangent).xyz;
	output.binormal = mul(world, binormal).xyz;
	output.tex = input.tex;

	output.lightViewPosition = mul(world, position);
	output.lightViewPosition = mul(lightViewProj, output.lightViewPosition);
	return output;
}