#include "Camera.h"
#include "Frustum.h"
#include "GameObject.h"
#include "Transform.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, Camera)
	Camera::Camera()
	{
		mFrustum = std::make_shared<Frustum>();
	}

	void Camera::initialize()
	{
		onEnable();
		setProjectionValues(60.0f, mAspectRatio, 0.1f, 75.0f);
	}

	void Camera::setFieldOfView(float angleInDegrees)
	{
		mFieldOfView = angleInDegrees;
	}

	void Camera::setAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
	}

	void Camera::setNearPlane(float nearPlane)
	{
		mNear = nearPlane;
	}

	void Camera::setFarPlane(float farPlane)
	{
		mFar = farPlane;
	}

	Frustum* Camera::getFrustum()
	{
		// Get the view matrix
		DirectX::XMFLOAT4X4 viewMatrix = mGameObject->getComponent<Transform>()->getGlobalTransform();

		// Calculate and return the frustum
		mFrustum->calculateFrustum(mFieldOfView, mAspectRatio, mNear, mFar, viewMatrix);
		return mFrustum.get();
	}

	const DirectX::XMFLOAT4X4 Camera::getInverseViewMatrix()
	{
		DirectX::XMFLOAT4X4 inverseView;

		if (Transform* transform = mGameObject->getComponent<Transform>())
		{
			inverseView = transform->getGlobalTransform();
			DirectX::XMStoreFloat4x4(&inverseView, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&inverseView)));
			return inverseView;
		}

		return inverseView;
	}

	void Camera::setProjectionValues(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		// Store the projection values
		mFieldOfView = fov;
		mAspectRatio = aspectRatio;
		mNear = nearPlane;
		mFar = farPlane;

		buildProjectionMatrix();
	}

	const DirectX::XMFLOAT4X4& Camera::getProjectionMatrix()
	{
		return mProjMatrix;
	}

	void Camera::buildProjectionMatrix()
	{
		// Calculate a projection matrix from the DirectX Math library
		DirectX::XMStoreFloat4x4(&mProjMatrix, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(mFieldOfView), mAspectRatio, mNear, mFar));

		// Get the view matrix
		DirectX::XMFLOAT4X4 viewMatrix;
		viewMatrix = mGameObject->getComponent<Transform>()->getGlobalTransform();
		mFrustum->calculateFrustum(mFieldOfView, mAspectRatio, mNear, mFar, viewMatrix);
	}
}