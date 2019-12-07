#include "Camera.h"
#include "Frustum.h"

namespace Odyssey
{
	Camera::Camera()
	{
		mPosition = { 0.0f, 0.0f, 0.0f };
		DirectX::XMVECTOR focal = { mPosition.x, mPosition.y, mPosition.z + 1.0f, 0.0f };
		DirectX::XMStoreFloat4x4(&mInverseViewMatrix, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&mPosition), focal, { 0,1,0,0 }));
		DirectX::XMStoreFloat4x4(&mViewMatrix, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mInverseViewMatrix)));
		mFrustum = std::make_shared<Frustum>();
	}

	void Camera::updateCamera(float x, float y, float z, float pitch, float yaw, float roll)
	{
		using namespace DirectX;

		// Create a rotation matrix from the pitch, yaw and roll
		XMMATRIX rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), XMConvertToRadians(roll));
		// Create a translation matrix from the delta x, y, and z
		XMMATRIX translation = XMMatrixTranslation(x, y, z);

		// Calculate a temporary new view matrix from the translation, rotation and current view matrix
		XMMATRIX viewMatrix = translation * rotation * XMLoadFloat4x4(&mViewMatrix);

		// From the new view matrix calculate a new x, y and z vector
		XMVECTOR zVector = XMVector3Normalize(viewMatrix.r[2]);
		XMVECTOR xVector = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), zVector));
		XMVECTOR yVector = XMVector3Normalize(XMVector3Cross(zVector, xVector));

		// Get the position vector from the view matrix
		XMVECTOR pVector = viewMatrix.r[3];

		// Set a new view matrix from the x, y, z directions and position vector
		mViewMatrix =
		{
			XMVectorGetX(xVector), XMVectorGetY(xVector), XMVectorGetZ(xVector), 0.0f,
			XMVectorGetX(yVector), XMVectorGetY(yVector), XMVectorGetZ(yVector), 0.0f,
			XMVectorGetX(zVector), XMVectorGetY(zVector), XMVectorGetZ(zVector), 0.0f,
			XMVectorGetX(pVector), XMVectorGetY(pVector), XMVectorGetZ(pVector), 1.0f
		};
		XMStoreFloat4x4(&mInverseViewMatrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&mViewMatrix)));
		mPosition = { mViewMatrix.m[3][0], mViewMatrix.m[3][1], mViewMatrix.m[3][2] };
		mFrustum->calculateFrustum(mFieldOfView, mAspectRatio, mNear, mFar, mViewMatrix);
	}

	void Camera::setPosition(float x, float y, float z)
	{
		mPosition = DirectX::XMFLOAT3(x, y, z);
		mViewMatrix.m[3][0] = x;
		mViewMatrix.m[3][1] = y;
		mViewMatrix.m[3][2] = z;

		updateCamera(0, 0, 0, 0, 0, 0);
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

	void Camera::setProjectionValues(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		// Store the projection values
		mFieldOfView = fov;
		mAspectRatio = aspectRatio;
		mNear = nearPlane;
		mFar = farPlane;

		buildProjectionMatrix();
	}

	DirectX::XMFLOAT3 Camera::getPosition()
	{
		return mPosition;
	}

	const DirectX::XMFLOAT4X4& Camera::getViewMatrix()
	{
		return mViewMatrix;
	}

	const DirectX::XMFLOAT4X4& Camera::getInverseViewMatrix()
	{
		return mInverseViewMatrix;
	}

	const DirectX::XMFLOAT4X4& Camera::getProjectionMatrix()
	{
		return mProjMatrix;
	}
	void Camera::buildProjectionMatrix()
	{
		// Calculate a projection matrix from the DirectX Math library
		DirectX::XMStoreFloat4x4(&mProjMatrix, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(mFieldOfView), mAspectRatio, mNear, mFar));

		mFrustum->calculateFrustum(mFieldOfView, mAspectRatio, mNear, mFar, mViewMatrix);
	}
}