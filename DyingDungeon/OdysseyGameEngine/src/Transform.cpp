#include "Transform.h"
#include "GameObject.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, Transform)

	Transform::Transform()
	{
		mPosition = DirectX::XMFLOAT3(0, 0, 0);
		mRotation = DirectX::XMFLOAT3(0, 0, 0);
		mScale = DirectX::XMFLOAT3(1, 1, 1);
		recalculateWorldMatrix();
	}

	Transform::Transform(DirectX::XMFLOAT4X4 worldMatrix)
	{
		mWorldMatrix = worldMatrix;
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
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;
		recalculateWorldMatrix();
	}

	void Transform::setPosition(float x, float y, float z)
	{
		mPosition = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
	}

	void Transform::addRotation(float x, float y, float z)
	{
		mRotation.x += x;
		mRotation.y += y;
		mRotation.z += z;
		recalculateWorldMatrix();
	}

	void Transform::setRotation(float x, float y, float z)
	{
		mRotation = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
	}

	void Transform::addScale(float x, float y, float z)
	{
		mScale.x += x;
		mScale.y += y;
		mScale.z += z;
		recalculateWorldMatrix();
	}

	void Transform::setScale(float x, float y, float z)
	{
		mScale = DirectX::XMFLOAT3(x, y, z);
		recalculateWorldMatrix();
	}

	DirectX::XMFLOAT3 Transform::getPosition()
	{
		return mPosition;
	}

	DirectX::XMFLOAT3 Transform::getRotation()
	{
		return mRotation;
	}

	DirectX::XMFLOAT3 Transform::getScale()
	{
		return mScale;
	}

	DirectX::XMFLOAT3 Transform::getForward()
	{
		return DirectX::XMFLOAT3(mWorldMatrix.m[2][0], mWorldMatrix.m[2][1], mWorldMatrix.m[2][2]);
	}

	DirectX::XMFLOAT3 Transform::getRight()
	{
		return DirectX::XMFLOAT3(mWorldMatrix.m[0][0], mWorldMatrix.m[0][1], mWorldMatrix.m[0][2]);
	}

	DirectX::XMFLOAT3 Transform::getUp()
	{
		return DirectX::XMFLOAT3(mWorldMatrix.m[1][0], mWorldMatrix.m[1][1], mWorldMatrix.m[1][2]);
	}

	void Transform::getLocalTransform(DirectX::XMFLOAT4X4& localTransform)
	{
		localTransform = mWorldMatrix;
	}

	void Transform::getGlobalTransform(DirectX::XMFLOAT4X4& globalTransform)
	{
		DirectX::XMMATRIX transform = DirectX::XMLoadFloat4x4(&mWorldMatrix);

		GameObject* parent = mGameObject->getParent();

		while (parent != nullptr)
		{
			DirectX::XMFLOAT4X4 localTransform;
			parent->getComponent<Transform>()->getLocalTransform(localTransform);
			transform = DirectX::XMMatrixMultiply(transform, DirectX::XMLoadFloat4x4(&localTransform));
			parent = parent->getParent();
		}

		DirectX::XMStoreFloat4x4(&globalTransform, transform);
	}

	void Transform::calculateEulerRotations()
	{
		if (mWorldMatrix.m[2][0] != 1 && mWorldMatrix.m[2][0] != -1)
		{
			float x = -std::asin(mWorldMatrix.m[2][0]);
			float cosX = std::cos(x);
			float y = std::atan2(mWorldMatrix.m[2][1] / cosX, mWorldMatrix.m[2][2] / cosX);
			float z = std::atan2(mWorldMatrix.m[1][0] / cosX, mWorldMatrix.m[0][0] / cosX);
			mRotation = DirectX::XMFLOAT3(x, y, z);
		}
		else
		{
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
			mRotation = DirectX::XMFLOAT3(x, y, z);
		}
	}

	void Transform::recalculateWorldMatrix()
	{
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(mRotation.x), DirectX::XMConvertToRadians(mRotation.y), DirectX::XMConvertToRadians(mRotation.z));
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
		DirectX::XMMATRIX world = DirectX::XMMatrixMultiply(scale, DirectX::XMMatrixMultiply(rotation, translation));
		DirectX::XMStoreFloat4x4(&mWorldMatrix, world);
	}
}