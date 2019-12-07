#include "VertexShaderIncludes.hlsli"

struct Particle
{
	float3 origin;
	float3 position;
	float3 velocity;
	float3 color;
	float lifeTime;
};

struct GSOutput
{
	float3 origin : ORIGIN;
	float4 position : SV_Position;
	float3 velocity : VELOCITY;
	float3 color : COLOR;
	float lifeTime : LIFETIME;
};

StructuredBuffer<Particle> pBufferIn : register(t0);
[maxvertexcount(4)]
void main(point uint input[1] : PRIMITIVE_ID, inout TriangleStream<GSOutput> output)
{
	Particle p = pBufferIn[input[0]];

	// Offsets to get the corner of each position
	// bottom left 0
	float4 offsets[4] =
	{
		float4(-1.0f, -1.0f, 0.0f, 0.0f), // LOWER LEFT
		float4(+1.0f, -1.0f, 0.0f, 0.0f), // LOWER RIGHT
		float4(+1.0f, +1.0f, 0.0f, 0.0f), // UPPER RIGHT
		float4(-1.0f, +1.0f, 0.0f, 0.0f) // UPPER LEFT
	};
	float size = 0.25f;
	float3 camRight = float3(view[0].xyz);
	float3 camUp = float3(view[1].xyz);
	GSOutput vOut[4];
	for (int i = 0; i < 4; i++)
	{
		float4 pos = float4(p.position + camRight * offsets[i].x * size + camUp * offsets[i].y * size, 1.0f);
		vOut[i].position = mul(worldViewProj, pos);
		float3 uv;
		uv.x = max(offsets[i].x + 1.0f, 1.0f);
		uv.y = max(offsets[i].y + 1.0f, 1.0f);
		uv.z = 0.0f;
		vOut[i].origin = uv.xyz;
		vOut[i].velocity = p.velocity;
		vOut[i].color = p.color;
		vOut[i].lifeTime = p.lifeTime;
	}

	output.Append(vOut[3]);
	output.Append(vOut[2]);
	output.Append(vOut[0]);
	output.Append(vOut[1]);
}