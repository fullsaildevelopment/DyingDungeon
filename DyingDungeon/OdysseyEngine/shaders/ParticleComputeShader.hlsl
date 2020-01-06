struct Particle
{
	float3 origin;
	float3 position;
	float3 velocity;
	float3 color;
	float lifeTime;
};

// Read/Write output
RWStructuredBuffer<Particle> pBufferOut : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
	pBufferOut[id.x].lifeTime = pBufferOut[id.x].lifeTime - .01f;
	pBufferOut[id.x].origin = pBufferOut[id.x].origin;
	float3 speed = pBufferOut[id.x].velocity * 0.01f;
	pBufferOut[id.x].position = pBufferOut[id.x].position + speed;
	pBufferOut[id.x].velocity = pBufferOut[id.x].velocity;

	//pBufferOut[id.x].position += pBufferIn[id.x].velocity * deltaTime;

	if (pBufferOut[id.x].lifeTime < 0)
	{
		pBufferOut[id.x].position = pBufferOut[id.x].origin;
		pBufferOut[id.x].velocity = pBufferOut[id.x].velocity;
		pBufferOut[id.x].lifeTime = 2.0f;
	}
}