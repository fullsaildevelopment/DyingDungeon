#include "Light.h"
#include "DebugManager.h"

namespace Odyssey
{
	void Light::debugDraw()
	{
		DirectX::XMFLOAT3 pos = { mWorldPosition.x, mWorldPosition.y, mWorldPosition.z };

		float range = (mLightType != LightType::Point) ? 10.0f : mRange;
		DirectX::XMFLOAT3 color = { mColor.x, mColor.y, mColor.z };
		DebugManager::getInstance().addSphere(pos, range, color);
	}

	DirectX::XMFLOAT4X4 Light::buildLightTransform(float sceneRadius, DirectX::XMFLOAT3 sceneCenter)
	{
		float distance = -2.0f * sceneRadius;
		DirectX::XMVECTOR lightPos = { mWorldDirection.x * distance, mWorldDirection.y * distance, mWorldDirection.z * distance, 1.0f };

		DirectX::XMStoreFloat4(&mWorldPosition, lightPos);

		DirectX::XMVECTOR targetPos = { 0.0f, 0.0f, 0.0f, 1.0f };
		DirectX::XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(lightPos, targetPos, up);

		DirectX::XMVECTOR sphereCenterLS = DirectX::XMVector3TransformCoord(targetPos, view);

		float l = DirectX::XMVectorGetX(sphereCenterLS) - sceneRadius;
		float b = DirectX::XMVectorGetY(sphereCenterLS) - sceneRadius;
		float n = DirectX::XMVectorGetZ(sphereCenterLS) - sceneRadius;

		float r = DirectX::XMVectorGetX(sphereCenterLS) + sceneRadius;
		float t = DirectX::XMVectorGetY(sphereCenterLS) + sceneRadius;
		float f = DirectX::XMVectorGetZ(sphereCenterLS) + sceneRadius;

		DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(view, proj);
		DirectX::XMFLOAT4X4 retMat;
		DirectX::XMStoreFloat4x4(&retMat, viewProj);
		return retMat;
	}

	void Light::setPosition(float x, float y, float z)
	{
		mWorldPosition = DirectX::XMFLOAT4(x, y, z, 1.0f);
	}

	void Light::addPosition(float x, float y, float z)
	{
		mWorldPosition.x += x;
		mWorldPosition.y += y;
		mWorldPosition.z += z;
	}
	void Light::setDirection(float x, float y, float z)
	{
		mWorldDirection = DirectX::XMFLOAT4(x, y, z, 0.0f);
	}
	void Light::addDirection(float x, float y, float z)
	{
		mWorldDirection.x += x;
		mWorldDirection.y += y;
		mWorldDirection.z += z;
	}
	void Light::rotateDirectionX(float angleInDegrees)
	{
		DirectX::XMVECTOR rotatedDirection = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&mWorldDirection), DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angleInDegrees)));
		DirectX::XMStoreFloat4(&mWorldDirection, rotatedDirection);
	}

	void Light::rotateDirectionY(float angleInDegrees)
	{

		DirectX::XMVECTOR rotatedDirection = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&mWorldDirection), DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleInDegrees)));
		DirectX::XMStoreFloat4(&mWorldDirection, rotatedDirection);
	}
	void Light::rotateDirectionZ(float angleInDegrees)
	{

		DirectX::XMVECTOR rotatedDirection = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&mWorldDirection), DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angleInDegrees)));
		DirectX::XMStoreFloat4(&mWorldDirection, rotatedDirection);
	}
	DirectX::XMFLOAT3 Light::getPosition()
	{
		return { mWorldPosition.x, mWorldPosition.y, mWorldPosition.z };
	}
}