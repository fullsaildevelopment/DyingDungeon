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

struct LightOutput
{
	float4 diffuse;
	float4 specular;
};

// Forward declarations - Light Calculations
LightOutput calculateLighting(float3 viewNormal, float3 surfaceNormal, float3 worldPosition, float4 lightViewPosition);
LightOutput calculateDirectionalLight(Light light, float3 viewVector, float3 surfaceNormal, float4 lightViewPosition);
LightOutput calculatePointLight(Light light, float3 viewVector, float3 worldPos, float3 surfaceNormal);
LightOutput calculateSpotLight(Light light, float3 viewVector, float3 worldPos, float3 surfaceNormal);

float calculateShadowFactor(float3 lightDirection, float3 surfaceNormal, float4 shadowPos);

// Forward declarations - Helper Functions
float4 calculateDiffuse(Light light, float3 lightVector, float3 surfaceNormal);
float4 calculateSpecular(Light light, float3 viewVector, float3 lightVector, float3 surfaceNormal);
float calculateAttenuation(Light light, float distance);
float calculateSpot(Light light, float3 lightVector);

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
		input.tangent = normalize(input.tangent);
		input.binormal = normalize(input.binormal);

		float4 texNormal = txNormal.Sample(samLinear, input.tex);
		// Convert from a 0.0 to 1.0 value into a -1.0 to 1.0 value
		texNormal = (2.0f * texNormal) - 1.0f;
		// Ensure the tangent is orthogonal
		input.tangent = normalize(input.tangent - dot(input.tangent, surfaceNormal) * surfaceNormal);
		// Create the texture space matrix
		float3x3 texSpace = float3x3(input.tangent, input.binormal, surfaceNormal);
		// Transform the texture normal into texture space
		surfaceNormal = normalize(mul(texNormal.xyz, texSpace));
	}

	// Calculate the surface normal and view normal
	float3 viewNormal = normalize(camPos.xyz - input.worldPosition);

	// Calculate the total scene lighting
	LightOutput sceneLighting = calculateLighting(viewNormal, surfaceNormal, input.worldPosition, input.lightViewPosition);

	// Calculate the diffuse color
    float4 diffuseColor = sceneLighting.diffuse * mat.diffuseColor;
	// Calculate the emissive color
	float4 emissiveColor = texEmissive;
	// Calculate the ambient color
	float4 ambientColor = mat.globalAmbient;
	// Calculate the specular color
	float4 specColor = sceneLighting.specular * (texSpecular + mat.specularColor).r;

	// Add together the different calculated colors
    float4 finalColor = (diffuseColor + emissiveColor + specColor + ambientColor) * texDiffuse;
	finalColor.a = texDiffuse.a;
	// Saturate and return the pixel color
    return finalColor;
}

LightOutput calculateLighting(float3 viewNormal, float3 surfaceNormal, float3 worldPosition, float4 lightViewPosition)
{
	LightOutput totalLighting = (LightOutput)0;

	// Iterate through the bound lights and calculate the total lighting on the surface
	for (unsigned int i = 0; i < numLights; i++)
	{
		LightOutput output = (LightOutput)0;

		switch (sceneLights[i].lightType)
		{
			// Calculate directional lighting including shadows
		case DIRECTIONAL_LIGHT:
		{
			output = calculateDirectionalLight(sceneLights[i], viewNormal, surfaceNormal, lightViewPosition);
		}
		break;
		// Calculate point lighting including point attenuation
		case POINT_LIGHT:
		{
			output = calculatePointLight(sceneLights[i], viewNormal, worldPosition, surfaceNormal);
		}
		break;
		// Calculate spot lighting including spot attenuations
		case SPOT_LIGHT:
		{
			output = calculateSpotLight(sceneLights[i], viewNormal, worldPosition, surfaceNormal);
		}
		break;
		}
		// Add in to the total lighting
		totalLighting.diffuse += output.diffuse;
		totalLighting.specular += output.specular;
	}
	return totalLighting;
}

// Calculates the diffuse and specular lighting of a directional light
LightOutput calculateDirectionalLight(Light light, float3 viewVector, float3 surfaceNormal, float4 lightViewPosition)
{
	LightOutput output;

	// Negate and normalize the light's direction
	float3 lightVector = -normalize(light.worldDirection.xyz);
	// Calculate the shadow factor of the directional light
	float shadowFactor = calculateShadowFactor(lightVector, surfaceNormal, lightViewPosition);
	// Calculate the diffuse of the directional light with the shadow factor multiplied in
	output.diffuse = calculateDiffuse(light, lightVector, surfaceNormal) * shadowFactor;
	output.specular = light.color * calculateSpecular(light, viewVector, lightVector, surfaceNormal) * shadowFactor;

	return output;
}

// Calculates the shadow factor of a directional light based on a shadow map texture
float calculateShadowFactor(float3 lightDirection, float3 surfaceNormal, float4 shadowPos)
{
	// If shadows are disabled the shadow factor is 1
	if (shadowsEnabled.x != 1)
	{
		return 1.0f;
	}

	// Perform the perspective divide on the shadow position
	shadowPos.x = 0.5f + (shadowPos.x / shadowPos.w * 0.5f);
	shadowPos.y = 0.5f - (shadowPos.y / shadowPos.w * 0.5f);

	float depth = shadowPos.z / shadowPos.w;

	// Check if this pixel could ever be in shadow
	if ((saturate(shadowPos.x) == shadowPos.x) && (saturate(shadowPos.y) == shadowPos.y) && (depth > 0))
	{
		// Calculate the delta texel
		const float dx = 1.0f / 4096.0f;
		float percentLit = 0.0f;
		// Calculate and clamp the bias
		float bias = 0.001f * tan(acos(dot(lightDirection, surfaceNormal)));
		bias = clamp(bias, 0.0f, 0.01f);

		// Sample the surrounding 9 texels for a cleaner edge
		for (float x = -1.5f; x < 1.5f; x += 1.0f)
		{
			for (float y = -1.5f; y < 1.5f; y += 1.0f)
			{
				// Calculate the texel offset
				float2 offset = float2(x, y) * dx;

				// Sample the shadow map and add the depth to the percent lit
				percentLit += txShadow.SampleCmpLevelZero(samShadow, shadowPos.xy + offset, depth - bias).r;
			}
		}
		return (percentLit /= 9.0f);
	}

	return 1.0f;

}

// Calculates the diffuse and specular lighting of a point light
LightOutput calculatePointLight(Light light, float3 viewVector, float3 worldPos, float3 surfaceNormal)
{
	LightOutput output;

	// Calculate the light vector between the vertex position in world space and the light's position
	float3 lightVector = light.worldPosition.xyz - worldPos;
	// Get the distance between the two points
	float distance = length(lightVector);
	// Normalize the light vector
	lightVector = lightVector / distance;
	// Calculate the appropriate attenuation based on the distance
	float attenuation = calculateAttenuation(light, distance);
	// Calculate the diffuse of the point light with attenuation factored in
	output.diffuse = calculateDiffuse(light, lightVector, surfaceNormal) * attenuation;
	output.specular = calculateSpecular(light, viewVector, lightVector, surfaceNormal) * attenuation;

	return output;
}

// Calculates the diffuse and specular lighting of a spot light
LightOutput calculateSpotLight(Light light, float3 viewVector, float3 worldPos, float3 surfaceNormal)
{
	LightOutput output;

	// Calculate the light vector between the vertex position in world space and the light's position
	float3 lightVector = light.worldPosition.xyz - worldPos;
	// Get the distance between the two points
	float distance = length(lightVector);
	// Normalize the light vector
	lightVector = lightVector / distance;
	// Calculate the appropriate attenuation based on the distance
	float attenuation = calculateAttenuation(light, distance);
	// Calculate the spot light's intensity
	float spotlightIntensity = calculateSpot(light, lightVector);
	// Calculate the diffuse of the spot light with attenuation and spot intensity factored in
	output.diffuse = calculateDiffuse(light, lightVector, surfaceNormal) * attenuation * spotlightIntensity;
	output.specular = calculateSpecular(light, viewVector, lightVector, surfaceNormal) * attenuation * spotlightIntensity;
	return output;
}

// Calculates the total specular lighting of any given light type
float4 calculateSpecular(Light light, float3 viewVector, float3 lightVector, float3 surfaceNormal)
{
	// Calculate the half vector of the light vector and view vector
	float3 R = reflect(-lightVector, surfaceNormal);
	// Take the dot product of the surface normal and the half vector
	float RdotV = saturate(dot(R, viewVector));

	// Calculate the intensity of the specular lighting
	float intensity = pow(RdotV, mat.specularPower);
	// Find the light's ratio on the surface
	float lightRatio = saturate(dot(lightVector, surfaceNormal));

	// Return the specular intensity and the intensity of the specular highlight
	return saturate(intensity * lightRatio);
}

// Calculates the total diffuse lighting of any given light type
float4 calculateDiffuse(Light light, float3 lightVector, float3 surfaceNormal)
{
	// Calculate the intensity of the light using the dot between the surface normal and the light vector while factoring in the intensity of the light itself
	float intensity = max(0.0f, dot(surfaceNormal, lightVector));
	// Return the diffuse of the light multiplied by it's total intensity
	return float4(light.color.xyz * intensity * light.intensity, 1.0f);
}

// Calculates the attenuation factor of any point/spot light
float calculateAttenuation(Light light, float distance)
{
	// Calculate the attenuation of the light based on the distance and the light's range
	float attenuation = (1.0f - saturate(distance / light.range));
	// Return an exponential attenuation
	return (attenuation * attenuation);
}

// Calculates the spot factor of any spot light
float calculateSpot(Light light, float3 lightVector)
{
	// Calculate the min and max cos of the light's cone
	float minCos = cos(light.cone);
	float maxCos = (minCos + 1.0f) / 2.0f;
	// Get the angle between the light's direction and the negative light vector
	float cosAngle = dot(light.worldDirection.xyz, -lightVector);
	// Return a smoothed interpolation between the min and max cos with the given angle
	return smoothstep(minCos, maxCos, cosAngle);
}