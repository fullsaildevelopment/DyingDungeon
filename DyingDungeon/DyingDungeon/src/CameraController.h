#pragma once
#include "Component.h"
#include "Camera.h"

class CameraController : public Odyssey::Component
{
	CLASS_DECLARATION(CameraController)

public:
	CameraController();
	virtual void initialize();
	virtual void update(double deltaTime);
	~CameraController() = default;
private:
	void updateMovement(double deltaTime);
	void updateRotation(double deltaTime);
private:
	Odyssey::Camera* mMainCamera;
	float mMovementSpeed;
	float mRotationSpeed;
};