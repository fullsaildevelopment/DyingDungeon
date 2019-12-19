#include "CameraController.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Transform.h"
#include "MenuManager.h"

CLASS_DEFINITION(Odyssey::Component, CameraController)

CameraController::CameraController()
{
	mRotationSpeed = 100.0f;
	mMovementSpeed = 10.0f;
	mMainCamera = nullptr;
}

void CameraController::initialize()
{
	onEnable();
	mMainCamera = mGameObject->getComponent<Odyssey::Camera>();
}

void CameraController::update(double deltaTime)
{
	if (mMainCamera)
	{
		if (Odyssey::InputManager::getInstance().getKeyDown(VK_F2))
		{
			if (Odyssey::Transform* transform = mGameObject->getComponent<Odyssey::Transform>())
			{
				transform->setPosition(7.44f, 6.13f, 4.03f);
				transform->setRotation(23.5f, -129.55f, 0.0f);
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

	Odyssey::Transform* transform = mGameObject->getComponent<Odyssey::Transform>();
	if (transform == nullptr)
	{
		return;
	}

	DirectX::XMFLOAT3 fwd = transform->getForward();
	DirectX::XMFLOAT3 right = transform->getRight();
	DirectX::XMFLOAT3 up = transform->getUp();

	if (input.getKeyDown('W'))
	{
		velocity.x += fwd.x;
		velocity.y += fwd.y;
		velocity.z += fwd.z;
	}
	if (input.getKeyDown('S'))
	{
		velocity.x -= fwd.x;
		velocity.y -= fwd.y;
		velocity.z -= fwd.z;
	}
	if (input.getKeyDown('D'))
	{
		velocity.x += right.x;
		velocity.y += right.y;
		velocity.z += right.z;
	}
	if (input.getKeyDown('A'))
	{
		velocity.x -= right.x;
		velocity.y -= right.y;
		velocity.z -= right.z;
	}
	if (input.getKeyDown(VK_SPACE))
	{
		velocity.x += up.x;
		velocity.y += up.y;
		velocity.z += up.z;
	}
	if (input.getKeyDown('X'))
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

	Odyssey::Transform* transform = mGameObject->getComponent<Odyssey::Transform>();

	if (transform == nullptr)
	{
		return;
	}

	if (input.getKeyDown(VK_UP))
	{
		rotation.x -= mRotationSpeed;
	}
	if (input.getKeyDown(VK_DOWN))
	{
		rotation.x += mRotationSpeed;
	}
	if (input.getKeyDown(VK_RIGHT))
	{
		rotation.y += mRotationSpeed;
	}
	if (input.getKeyDown(VK_LEFT))
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
