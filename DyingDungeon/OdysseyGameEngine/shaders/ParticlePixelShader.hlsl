struct GSOutput
{
	float3 origin : ORIGIN;
	float4 position : SV_Position;
	float3 velocity : VELOCITY;
	float3 color : COLOR;
	float lifeTime : LIFETIME;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);
float4 main(GSOutput input) : SV_TARGET
{
	return float4(0.45, 0.15, 0.0f, input.lifeTime);
}