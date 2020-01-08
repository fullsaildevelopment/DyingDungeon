#include "CameraController.h"
#include "Entity.h"
#include "Camera.h"
#include "InputManager.h"
#include "Transform.h"

CLASS_DEFINITION(Odyssey::Component, CameraController)

CameraController::CameraController()
{
	mRotationSpeed = 100.0f;
	mMovementSpeed = 10.0f;
	mMainCamera = nullptr;
}

void CameraController::initialize()
{
	mMainCamera = mEntity->getComponent<Odyssey::Camera>();
}

void CameraController::update(double deltaTime)
{
	if (mMainCamera)
	{
		if (Odyssey::InputManager::getInstance().getKeyDown(KeyCode::F2))
		{
			if (Odyssey::Transform* transform = mEntity->getComponent<Odyssey::Transform>())
			{
				transform->setPosition(3.81f, 11.41f, 8.56f);
				transform->setRotation(41.82f, -158.32f, 0.0f);
			}
		}

		updateMovement(deltaTime);
		updateRotation(deltaTime);
	}
}

void CameraController::updateMovement(double deltaTime)
{
	Odyssey::InputManager& input = Odyssey::InputManager::getInstance();
	DirectX::XMFLOAT3 velocity(0.0f, 0.0f, 0.0f);

	Odyssey::Transform* transform = mEntity->getComponent<Odyssey::Transform>();
	if (transform == nullptr)
	{
		return;
	}

	DirectX::XMFLOAT3 fwd = transform->getForward();
	DirectX::XMFLOAT3 right = transform->getRight();
	DirectX::XMFLOAT3 up = transform->getUp();

	if (input.getKeyDown(KeyCode::W))
	{
		velocity.x += fwd.x;
		velocity.y += fwd.y;
		velocity.z += fwd.z;
	}
	if (input.getKeyDown(KeyCode::S))
	{
		velocity.x -= fwd.x;
		velocity.y -= fwd.y;
		velocity.z -= fwd.z;
	}
	if (input.getKeyDown(KeyCode::D))
	{
		velocity.x += right.x;
		velocity.y += right.y;
		velocity.z += right.z;
	}
	if (input.getKeyDown(KeyCode::A))
	{
		velocity.x -= right.x;
		velocity.y -= right.y;
		velocity.z -= right.z;
	}
	if (input.getKeyDown(KeyCode::Space))
	{
		velocity.x += up.x;
		velocity.y += up.y;
		velocity.z += up.z;
	}
	if (input.getKeyDown(KeyCode::X))
	{
		velocity.x -= up.x;
		velocity.y -= up.y;
		velocity.z -= up.z;
	}

	bool zero = velocity.x == 0.0f && velocity.y == 0.0f && velocity.z == 0.0f;
	if (!zero)
	{
		velocity.x = velocity.x * mMovementSpeed * static_cast<float>(deltaTime);
		velocity.y = velocity.y * mMovementSpeed * static_cast<float>(deltaTime);
		velocity.z = velocity.z * mMovementSpeed * static_cast<float>(deltaTime);
		transform->addPosition(velocity.x, velocity.y, velocity.z);
	}
}

void CameraController::updateRotation(double deltaTime)
{
	Odyssey::InputManager& input = Odyssey::InputManager::getInstance();
	DirectX::XMFLOAT3 rotation(0.0f, 0.0f, 0.0f);

	Odyssey::Transform* transform = mEntity->getComponent<Odyssey::Transform>();

	if (transform == nullptr)
	{
		return;
	}

	if (input.getKeyDown(KeyCode::Up))
	{
		rotation.x -= mRotationSpeed;
	}
	if (input.getKeyDown(KeyCode::Down))
	{
		rotation.x += mRotationSpeed;
	}
	if (input.getKeyDown(KeyCode::Right))
	{
		rotation.y += mRotationSpeed;
	}
	if (input.getKeyDown(KeyCode::Left))
	{
		rotation.y -= mRotationSpeed;
	}

	bool zero = rotation.x == 0.0f && rotation.y == 0.0f && rotation.z == 0.0f;

	if (!zero)
	{
		rotation.x = rotation.x * static_cast<float>(deltaTime);
		rotation.y = rotation.y * static_cast<float>(deltaTime);
		rotation.z = rotation.z * static_cast<float>(deltaTime);
		transform->addRotation(rotation.x, rotation.y, rotation.z);
	}
}
