struct Particle
{
	float3 origin;
	float3 position;
	float3 velocity;
	float3 color;
    float lifeTime;
    float startLifetime;
};

// Read/Write output
RWStructuredBuffer<Particle> pBufferOut : register(u0);

cbuffer ParticleValueBuffer : register(b1)
{
    float4 startVelocity;
    float4 startPosition;
    float4 startColor;
    float deltaTime;
    float startLifetime;
    float startSpeed;
    float duration;
};

[numthreads(256, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
	pBufferOut[id.x].lifeTime += deltaTime;
    pBufferOut[id.x].position += pBufferOut[id.x].velocity * startSpeed * deltaTime;

	//if (pBufferOut[id.x].lifeTime > pBufferOut[id.x].startLifetime)
	//{
	//	pBufferOut[id.x].position = pBufferOut[id.x].origin;
    //    pBufferOut[id.x].lifeTime = 0.0f;
    //}
}