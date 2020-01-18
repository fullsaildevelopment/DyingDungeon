#include "ParticleMover.h"


CLASS_DEFINITION(Component, ParticleMover)

void ParticleMover::initialize()
{
	mFuckYouMax = mEntity->getComponent<Odyssey::Transform>();
	mOrigin = mFuckYouMax->getPosition();
}

void ParticleMover::update(double deltaTime)
{
	mFuckYouMax->addPosition(mVelocity.x * deltaTime, mVelocity.y * deltaTime, mVelocity.z * deltaTime);
	static double tempTime = 0.0f;
	tempTime += deltaTime;
	if (tempTime > mLifeTime)
	{
		Reset();
		tempTime = 0.0f;
	}
}

Odyssey::ParticleSystem* ParticleMover::GetPS()
{
	return mPS;
}

void ParticleMover::SetPS(Odyssey::ParticleSystem* newPS)
{
	mPS = newPS;
}

DirectX::XMFLOAT3 ParticleMover::GetTargetPos()
{
	return mTargetPos;
}

void ParticleMover::SetTargetPos(DirectX::XMFLOAT3 newTargetPos)
{
	mTargetPos = newTargetPos;
	mVelocity = { (mTargetPos.x - mOrigin.x) * mSpeed, (mTargetPos.y - mOrigin.y) * mSpeed, (mTargetPos.z - mOrigin.z) * mSpeed };
}

DirectX::XMFLOAT3 ParticleMover::GetOriginPos()
{
	return mOrigin;
}

void ParticleMover::SetOrigin(DirectX::XMFLOAT3 newOrigin)
{
	mOrigin = newOrigin;
}

Odyssey::Transform* ParticleMover::GetPos()
{
	return mFuckYouMax;
}

void ParticleMover::SetPos(Odyssey::Transform* newPos)
{
	mFuckYouMax = newPos;
}

float ParticleMover::GetSpeed()
{
	return mSpeed;
}

void ParticleMover::SetSpeed(float newSpeed)
{
	mSpeed = newSpeed;
}

float ParticleMover::GetLifeTime()
{
	return mLifeTime;
}

void ParticleMover::SetLifeTime(float newLifeTime)
{
	mLifeTime = newLifeTime;
}

void ParticleMover::Reset()
{
	mEntity->setActive(false);
	mEntity->setVisible(false);
	mFuckYouMax->setPosition(mOrigin.x, mOrigin.y, mOrigin.z);
}
