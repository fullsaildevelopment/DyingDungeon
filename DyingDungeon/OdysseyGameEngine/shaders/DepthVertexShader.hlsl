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

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

cbuffer AnimationData : register(b1)
{
	float4 hasAnimationData;
	float4x4 finalPose[128];
};

PixelInputType main(VERTEX_SHADER_INPUT input)
{
	PixelInputType output;
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

	position.xyz = position.xyz - (normal.xyz * 0.001f);
	output.position = mul(world, position);
	output.position = mul(lightViewProj, output.position);
	output.depthPosition = output.position;
	return output;
}