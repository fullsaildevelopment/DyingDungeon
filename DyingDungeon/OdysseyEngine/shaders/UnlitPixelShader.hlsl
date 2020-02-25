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

	// Calculate the diffuse color
	float4 diffuseColor = mat.diffuseColor;
	// Calculate the emissive color
	float4 emissiveColor = texEmissive;
	// Calculate the ambient color
	float4 ambientColor = mat.globalAmbient;

	// Add together the different calculated colors
	float4 finalColor = (diffuseColor + emissiveColor + ambientColor) * texDiffuse;
	finalColor.a = texDiffuse.a;

	// Saturate and return the pixel color
	return saturate(finalColor);
}