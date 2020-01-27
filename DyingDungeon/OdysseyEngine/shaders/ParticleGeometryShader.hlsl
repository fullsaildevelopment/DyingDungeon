#include "VertexShaderIncludes.hlsli"

struct Particle
{
	float4 color;
	float3 position;
    float lifetime;
    float3 velocity;
    float startLifetime;
	float size;
	bool active;
};

struct GSOutput
{
	float4 color : COLOR;
	float4 position : SV_Position;
    float2 tex : TEXCOORD;
    bool active : ACTIVE;
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
    float2 tex[4] =
    {
        float2(0.0f, 1.0f), // LOWER LEFT
		float2(1.0f, 1.0f), // LOWER RIGHT
		float2(1.0f, 0.0f), // UPPER RIGHT
		float2(0.0f, 0.0f) // UPPER LEFT
    };
    float3 camRight = view[0].xyz;
    float3 camUp = view[1].xyz;

	GSOutput vOut[4];
	for (int i = 0; i < 4; i++)
	{
		float4 pos = float4(p.position + (camRight * offsets[i].x * (p.size * 0.5f)) + (camUp * offsets[i].y * (p.size * 0.5f)), 1.0f);
		vOut[i].position = mul(world, pos);
		vOut[i].position = mul(viewProj, vOut[i].position);
		vOut[i].color = p.color;
        vOut[i].tex = tex[i];
        vOut[i].active = p.active;
    }

	output.Append(vOut[3]);
	output.Append(vOut[2]);
	output.Append(vOut[0]);
	output.Append(vOut[1]);
}