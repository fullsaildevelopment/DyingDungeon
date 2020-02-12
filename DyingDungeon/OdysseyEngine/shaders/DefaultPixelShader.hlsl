#include "PixelShaderIncludes.hlsli"

struct PIXEL_SHADER_INPUT
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 tex : TEXCOORD;
	float4 lightViewPosition : POSITION1;
};

float4 main(PIXEL_SHADER_INPUT input) : SV_TARGET
{
	// Set default values for the possible bound textures
	float4 texDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 texEmissive = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 texSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Ensure the input normal is normalized
	float3 surfaceNormal = normalize(input.normal);

	// Sample the diffuse texture if one is bound
	if (mat.hasDiffuseTexture)
	{
		texDiffuse = txDiffuse.Sample(samLinear, input.tex);
	}
	// Sample the emissive texture if one is bound
	if (mat.hasEmissiveTexture)
	{
		texEmissive = txEmissive.Sample(samLinear, input.tex);
	}
	// Sample the specular texture if one is bound
	if (mat.hasSpecularTexture)
	{
		texSpecular = txSpecular.Sample(samLinear, input.tex);
	}

	// If a normal map is bound, transform the surface normal accordingly
	if (mat.hasNormalTexture)
	{
		// Sample the texture
		//input.tangent = normalize(input.tangent);
		//input.binormal = normalize(input.binormal);
		//
		//float4 texNormal = txNormal.Sample(samLinear, input.tex);
		//// Convert from a 0.0 to 1.0 value into a -1.0 to 1.0 value
		//texNormal = (2.0f * texNormal) - 1.0f;
		//// Ensure the tangent is orthogonal
		//input.tangent = normalize(input.tangent - dot(input.tangent, surfaceNormal) * surfaceNormal);
		//// Create the texture space matrix
		//float3x3 texSpace = float3x3(input.tangent, input.binormal, surfaceNormal);
		//// Transform the texture normal into texture space
		//surfaceNormal = normalize(mul(texNormal.xyz, texSpace));
	}

	return float4(surfaceNormal, 1.0f);
}