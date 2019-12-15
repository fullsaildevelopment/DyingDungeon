#include "Transform.h"
#include "GameObject.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, Transform)

	Transform::Transform()
	{
		// Set default values for the transform
		mPosition = DirectX::XMFLOAT3(0, 0, 0);
		mRotation = DirectX::XMFLOAT3(0, 0, 0);
		mScale = DirectX::XMFLOAT3(1, 1, 1);
		recalculateWorldMatrix();
	}

	Transform::Transform(DirectX::XMFLOAT4X4 worldMatrix)
	{
		// Assign the world matrix from the parameter
		mWorldMatrix = worldMatrix;

		// Decompose the world matrix into it's position, rotation and scale components and assign them
		mPosition = { mWorldMatrix.m[3][0], mWorldMatrix.m[3][1], mWorldMatrix.m[3][2] };
		mScale = { std::fabsf(mWorldMatrix.m[0][0]), std::fabsf(mWorldMatrix.m[1][1]), std::fabsf(mWorldMatrix.m[2][2]) };
		calculateEulerRotations();
	}

	void Transform::initialize()
	{
		onEnable();
	}

	void Transform::addPosition(float x, float y, float z)
	{
		// Add to the x, y, z components of the position and calculate a new world matrix
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;
		recalculateWorldMatrix();
	}

	void Transform::setPosition(float x, float y, float z)
	{
		// Set the new position and calculate a new world matrix
		mPosition = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
	}

	void Transform::getPosition(DirectX::XMFLOAT3& position)
	{
		// Set the out parameter to the position
		position = mPosition;
	}

	void Transform::addRotation(float x, float y, float z)
	{
		// Add to the x, y, z components of the rotation and calculate a new world matrix
		mRotation.x += x;
		mRotation.y += y;
		mRotation.z += z;
		recalculateWorldMatrix();
	}

	void Transform::setRotation(float x, float y, float z)
	{
		// Set the new rotation and calculate a new world matrix
		mRotation = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
	}

	void Transform::getEulerRotation(DirectX::XMFLOAT3& rotation)
	{
		// Set the out parameter to the rotation
		rotation = mRotation;
	}

	void Transform::addScale(float x, float y, float z)
	{
		// Add to the x, y, z components of the scale and calculate a new world matrix
		mScale.x += x;
		mScale.y += y;
		mScale.z += z;
		recalculateWorldMatrix();
	}

	void Transform::setScale(float x, float y, float z)
	{
		// Set the new scale and calculate a new world matrix
		mScale = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
	}

	void Transform::getScale(DirectX::XMFLOAT3& scale)
	{
		// Set the out parameter to the scale
		scale = mScale;
	}

	void Transform::getForward(DirectX::XMFLOAT3& forward)
	{
		// Set the out parameter to the forward vector from the world matrix
		forward = DirectX::XMFLOAT3(mWorldMatrix.m[2][0], mWorldMatrix.m[2][1], mWorldMatrix.m[2][2]);
	}

	void Transform::getRight(DirectX::XMFLOAT3& right)
	{
		// Set the out parameter to the right vector from the world matrix
		right = DirectX::XMFLOAT3(mWorldMatrix.m[0][0], mWorldMatrix.m[0][1], mWorldMatrix.m[0][2]);
	}
	
	void Transform::getUp(DirectX::XMFLOAT3& up)
	{
		// Set the out parameter to the up vector from the world matrix
		up = DirectX::XMFLOAT3(mWorldMatrix.m[1][0], mWorldMatrix.m[1][1], mWorldMatrix.m[1][2]);
	}

	void Transform::getLocalTransform(DirectX::XMFLOAT4X4& localTransform)
	{
		// Set the out parameter to the local-space world matrix
		localTransform = mWorldMatrix;
	}

	void Transform::getGlobalTransform(DirectX::XMFLOAT4X4& globalTransform)
	{
		// Store the world matrix in an XMMatrix
		DirectX::XMMATRIX transform = DirectX::XMLoadFloat4x4(&mWorldMatrix);

		// Get the game object we are attached to's parent
		GameObject* parent = mGameObject->getParent();

		// Ensure a parent still exists
		while (parent != nullptr)
		{
			// Get the parent's local transform
			DirectX::XMFLOAT4X4 localTransform;
			parent->getComponent<Transform>()->getLocalTransform(localTransform);

			// Move our world matrix into the parent's local-space
			transform = DirectX::XMMatrixMultiply(transform, DirectX::XMLoadFloat4x4(&localTransform));

			// Get the next parent
			parent = parent->getParent();
		}

		// Set the out paramter to the global-space world matrix
		DirectX::XMStoreFloat4x4(&globalTransform, transform);
	}

	void Transform::calculateEulerRotations()
	{
		// Check if the row 3 column 1 is +- 1
		if (mWorldMatrix.m[2][0] != 1 && mWorldMatrix.m[2][0] != -1)
		{
			// Calculate the euler angle rotation from the world matrix
			float x = -std::asin(mWorldMatrix.m[2][0]);
			float cosX = std::cos(x);
			float y = std::atan2(mWorldMatrix.m[2][1] / cosX, mWorldMatrix.m[2][2] / cosX);
			float z = std::atan2(mWorldMatrix.m[1][0] / cosX, mWorldMatrix.m[0][0] / cosX);

			// Set the calculated rotation value
			mRotation = DirectX::XMFLOAT3(x, y, z);
		}
		else
		{

			// Calculate the euler angle rotation from the world matrix
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			if (mWorldMatrix.m[2][0] == -1)
			{
				x = DirectX::g_XMPi.f[0] / 2.0f;
				y = x + std::atan2(mWorldMatrix.m[0][1], mWorldMatrix.m[0][2]);
			}
			else
			{
				x = -DirectX::g_XMPi.f[0] / 2.0f;
				y = -x + std::atan2(-mWorldMatrix.m[0][1], -mWorldMatrix.m[0][2]);
			}

			// Set the calculated rotation value
			mRotation = DirectX::XMFLOAT3(x, y, z);
		}
	}

	void Transform::recalculateWorldMatrix()
	{
		// Compose the position, rotation and scale into 3 matrices
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(mRotation.x), DirectX::XMConvertToRadians(mRotation.y), DirectX::XMConvertToRadians(mRotation.z));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);

		// Calculate and store the world matrix = scale * rotation * translation
		DirectX::XMMATRIX world = DirectX::XMMatrixMultiply(scale, DirectX::XMMatrixMultiply(rotation, translation));
		DirectX::XMStoreFloat4x4(&mWorldMatrix, world);
	}
}