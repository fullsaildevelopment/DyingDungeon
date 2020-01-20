#include "Camera.h"
#include "Frustum.h"
#include "Entity.h"
#include "Transform.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, Camera)

	Camera::Camera()
	{
		// Create a frustum and set default projection values
		mFrustum = std::make_shared<Frustum>();
	}

	void Camera::initialize()
	{
		setProjectionValues(60.0f, mAspectRatio, 0.1f, 1500.0f);
	}

	void Camera::setFieldOfView(float angleInDegrees)
	{
		// Set the field of view in degrees
		mFieldOfView = angleInDegrees;
	}

	void Camera::setAspectRatio(float aspectRatio)
	{
		// Set the aspect ratio
		mAspectRatio = aspectRatio;
	}

	void Camera::setNearPlane(float nearPlane)
	{
		// Set the near plane
		mNear = nearPlane;
	}

	void Camera::setFarPlane(float farPlane)
	{
		// Set the far plane
		mFar = farPlane;
	}

	Frustum* Camera::getFrustum()
	{
		// Get the view matrix
		DirectX::XMFLOAT4X4 viewMatrix = mEntity->getComponent<Transform>()->getGlobalTransform();

		// Calculate and return the frustum
		mFrustum->calculateFrustum(mFieldOfView, mAspectRatio, mNear, mFar, viewMatrix);
		return mFrustum.get();
	}

	const DirectX::XMFLOAT4X4 Camera::getInverseViewMatrix()
	{
		// Create a container for the inverse view matrix and default it with identity
		DirectX::XMFLOAT4X4 inverseView;
		DirectX::XMStoreFloat4x4(&inverseView, DirectX::XMMatrixIdentity());

		// Check if the entity has a transform component
		if (Transform* transform = mEntity->getComponent<Transform>())
		{
			// Inverse the entity's global transform and return it as the inverse view
			inverseView = transform->getGlobalTransform();
			DirectX::XMStoreFloat4x4(&inverseView, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&inverseView)));
			return inverseView;
		}

		// Return the default identity
		return inverseView;
	}

	void Camera::setProjectionValues(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		// Store the projection values
		mFieldOfView = fov;
		mAspectRatio = aspectRatio;
		mNear = nearPlane;
		mFar = farPlane;

		// Rebuild the projection matrix
		buildProjectionMatrix();
	}

	const DirectX::XMFLOAT4X4 Camera::getProjectionMatrix()
	{
		// Return the projection matrix
		return mProjMatrix;
	}

	void Camera::buildProjectionMatrix()
	{
		// Calculate a FoV LH projection matrix
		DirectX::XMStoreFloat4x4(&mProjMatrix, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(mFieldOfView), mAspectRatio, mNear, mFar));

		// Get the view matrix
		DirectX::XMFLOAT4X4 viewMatrix;
		viewMatrix = mEntity->getComponent<Transform>()->getGlobalTransform();

		// Recalculate the frustum
		mFrustum->calculateFrustum(mFieldOfView, mAspectRatio, mNear, mFar, viewMatrix);
	}
}