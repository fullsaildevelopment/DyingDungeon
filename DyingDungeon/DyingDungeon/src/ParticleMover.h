#include "OdysseyEngine.h"
#pragma once
class ParticleMover : public Odyssey::Component
{
	CLASS_DECLARATION(ParticleMover)
public:
private:
	Odyssey::ParticleSystem* mPS;
	DirectX::XMFLOAT3 mTargetPos;
	DirectX::XMFLOAT3 mOrigin;
	DirectX::XMFLOAT3 mVelocity;
	Odyssey::Transform* mFuckYouMax;
	float mSpeed;
	float mLifeTime;

public:
	ParticleMover() = default;
	~ParticleMover() = default;
	virtual void initialize();
	virtual void update(double deltaTime);
	Odyssey::ParticleSystem* GetPS();
	void SetPS(Odyssey::ParticleSystem* newPS);
	DirectX::XMFLOAT3 GetTargetPos();
	void SetTargetPos(DirectX::XMFLOAT3 newTargetPos);
	DirectX::XMFLOAT3 GetOriginPos();
	void SetOrigin(DirectX::XMFLOAT3 newOrigin);
	Odyssey::Transform* GetPos();
	void SetPos(Odyssey::Transform* newPos);
	float GetSpeed();
	void SetSpeed(float newSpeed);
	float GetLifeTime();
	void SetLifeTime(float newLifeTime);
	DirectX::XMFLOAT3 GetVelocity();
	void Reset();
private:
};

