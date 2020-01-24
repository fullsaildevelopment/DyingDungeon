#include "Transform.h"
#include "Entity.h"

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

	void Transform::addPosition(float x, float y, float z)
	{
		// Add to the x, y, z components of the position and calculate a new world matrix
		mLock.lock(LockState::Write);
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;
		recalculateWorldMatrix();
		mLock.unlock(LockState::Write);
	}

	void Transform::setPosition(float x, float y, float z)
	{
		// Set the new position and calculate a new world matrix
		mLock.lock(LockState::Write);
		mPosition = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
		mLock.unlock(LockState::Write);
	}

	DirectX::XMFLOAT3 Transform::getPosition()
	{
		// Set the out parameter to the position
		mLock.lock(LockState::Read);
		DirectX::XMFLOAT3 position = mPosition;
		mLock.unlock(LockState::Read);
		return position;
	}

	void Transform::addRotation(float x, float y, float z)
	{
		// Add to the x, y, z components of the rotation and calculate a new world matrix
		mLock.lock(LockState::Write);
		mRotation.x += x;
		mRotation.y += y;
		mRotation.z += z;
		recalculateWorldMatrix();
		mLock.unlock(LockState::Write);
	}

	void Transform::setRotation(float x, float y, float z)
	{
		// Set the new Write and calculate a new world matrix
		mLock.lock(LockState::Read);
		mRotation = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
		mLock.unlock(LockState::Read);
	}

	DirectX::XMFLOAT3 Transform::getEulerRotation()
	{
		// Return the rotation value
		mLock.lock(LockState::Read);
		DirectX::XMFLOAT3 rotation = mRotation;
		mLock.unlock(LockState::Read);
		return rotation;
	}

	void Transform::addScale(float x, float y, float z)
	{
		// Add to the x, y, z components of the scale and calculate a new world matrix
		mLock.lock(LockState::Write);
		mScale.x += x;
		mScale.y += y;
		mScale.z += z;
		recalculateWorldMatrix();
		mLock.unlock(LockState::Write);
	}

	void Transform::setScale(float x, float y, float z)
	{
		// Set the new scale and calculate a new world matrix
		mLock.lock(LockState::Write);
		mScale = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
		mLock.unlock(LockState::Write);
	}

	DirectX::XMFLOAT3 Transform::getScale()
	{
		// Return the scale value
		mLock.lock(LockState::Read);
		DirectX::XMFLOAT3 scale = mScale;
		mLock.unlock(LockState::Read);
		return scale;
	}

	DirectX::XMFLOAT3 Transform::getForward()
	{
		// Set the out parameter to the forward vector from the world matrix
		mLock.lock(LockState::Read);
		DirectX::XMVECTOR fwd = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(mWorldMatrix.m[2][0], mWorldMatrix.m[2][1], mWorldMatrix.m[2][2])));
		mLock.unlock(LockState::Read);
		DirectX::XMFLOAT3 f3Fwd;
		DirectX::XMStoreFloat3(&f3Fwd, fwd);
		return f3Fwd;
	}

	DirectX::XMFLOAT3 Transform::getRight()
	{
		// Set the out parameter to the right vector from the world matrix
		mLock.lock(LockState::Read);
		DirectX::XMFLOAT3 right = DirectX::XMFLOAT3(mWorldMatrix.m[0][0], mWorldMatrix.m[0][1], mWorldMatrix.m[0][2]);
		mLock.unlock(LockState::Read);
		return right;
	}
	
	DirectX::XMFLOAT3 Transform::getUp()
	{
		// Set the out parameter to the up vector from the world matrix
		mLock.lock(LockState::Read);
		DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(mWorldMatrix.m[1][0], mWorldMatrix.m[1][1], mWorldMatrix.m[1][2]);
		mLock.unlock(LockState::Read);
		return up;
	}

	DirectX::XMFLOAT4X4 Transform::getLocalTransform(bool ignoreScale)
	{
		// Set the out parameter to the local-space world matrix
		if (ignoreScale)
		{
			DirectX::XMFLOAT3 tScale = mScale;
			mScale = { 1.0f, 1.0f, 1.0f };
			recalculateWorldMatrix();
			DirectX::XMFLOAT4X4 nonScaled = mWorldMatrix;
			mScale = tScale;
			recalculateWorldMatrix();
			return nonScaled;
		}
		DirectX::XMFLOAT4X4 world = mWorldMatrix;

		return world;
	}

	DirectX::XMFLOAT4X4 Transform::getGlobalTransform(bool ignoreScale)
	{
		// Store the world matrix in an XMMatrix
		mLock.lock(LockState::Read);
		DirectX::XMMATRIX transform = DirectX::XMLoadFloat4x4(&getLocalTransform(ignoreScale));

		// Get the entity we are attached to's parent
		Entity* parent = mEntity->getParent();

		// Ensure a parent still exists
		while (parent != nullptr)
		{
			// Get the parent's local transform
			DirectX::XMFLOAT4X4 localTransform = parent->getComponent<Transform>()->getLocalTransform(ignoreScale);

			// Move our world matrix into the parent's local-space
			transform = DirectX::XMMatrixMultiply(transform, DirectX::XMLoadFloat4x4(&localTransform));

			// Get the next parent
			parent = parent->getParent();
		}

		mLock.unlock(LockState::Read);

		// Set the out paramter to the global-space world matrix
		DirectX::XMFLOAT4X4 retTransform;
		DirectX::XMStoreFloat4x4(&retTransform, transform);
		return retTransform;
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