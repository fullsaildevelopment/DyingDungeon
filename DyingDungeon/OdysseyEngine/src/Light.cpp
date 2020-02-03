#include "Light.h"
#include "DebugManager.h"
#include "Entity.h"
#include "Transform.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, Light)

	Light::Light()
	{

	}

	void Light::initialize()
	{
		mTransform = mEntity->getComponent<Transform>();

		DirectX::XMFLOAT3 holder = mTransform->getPosition();
		mPosition = { holder.x, holder.y, holder.z, 1.0f };
		holder = mTransform->getForward();
		mDirection = { holder.x, holder.y, holder.z, 0.0f };
	}

	void Light::debugDraw()
	{
		// Directional and spot use a range of 10, point light use it's own range
		float range = (mLightType != LightType::Point) ? 10.0f : mRange;

		// Convert the position and color to XMFloat3
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(mPosition.x, mPosition.y, mPosition.z);
		DirectX::XMFLOAT3 color = DirectX::XMFLOAT3(mColor.x, mColor.y, mColor.z);

		// Draw a sphere at the position of the light with a radius of matching the range of the light
		DebugManager::getInstance().addSphere(position, range, color);
	}

	void Light::buildLightTransform(DirectX::XMFLOAT3& sceneCenter, float sceneRadius, DirectX::XMFLOAT4X4& lightTransform)
	{
		// Calculate the distance to calculate the light's position
		float distance = -2.0f * sceneRadius;

		// Calculate the light's position
		mPosition = { mDirection.x * distance, mDirection.y * distance, mDirection.z * distance, 1.0f};

		// Calculate and store a view matrix for the light using the light's position, the scene center and default up
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&mPosition), DirectX::XMLoadFloat3(&sceneCenter), { 0.0f, 1.0f, 0.0f, 0.0f });
		DirectX::XMStoreFloat4x4(&lightTransform, view);
	}

	void Light::buildLightTransformProjection(DirectX::XMFLOAT3& sceneCenter, float sceneRadius, DirectX::XMFLOAT4X4& lightTransform)
	{
		// Calculate the distance to calculate the light's position
		float distance = -2.0f * sceneRadius;

		// Calculate the light's position
		mPosition = { mDirection.x * distance, mDirection.y * distance, mDirection.z * distance, 1.0f };

		// Calculate a view matrix for the light using the light's position, the scene center and default up
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&mPosition), DirectX::XMLoadFloat3(&sceneCenter), { 0.0f, 1.0f, 0.0f, 0.0f });

		// Get the sphere's center position in light-space.
		DirectX::XMVECTOR sphereCenterLS = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&sceneCenter), view);

		// Calculate the projection bounds for the orthographic projection
		float l = DirectX::XMVectorGetX(sphereCenterLS) - sceneRadius;
		float b = DirectX::XMVectorGetY(sphereCenterLS) - sceneRadius;
		float n = DirectX::XMVectorGetZ(sphereCenterLS) - sceneRadius;
		float r = DirectX::XMVectorGetX(sphereCenterLS) + sceneRadius;
		float t = DirectX::XMVectorGetY(sphereCenterLS) + sceneRadius;
		float f = DirectX::XMVectorGetZ(sphereCenterLS) + sceneRadius;

		// Calculate an orthographic projection matrix for the light
		DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

		// Calculate the view projection matrix and return it
		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(view, proj);
		DirectX::XMStoreFloat4x4(&lightTransform, viewProj);
	}

	DirectX::XMFLOAT4 Light::getPosition()
	{
		// Set the out parameter to the position
		DirectX::XMFLOAT3 pos = mTransform->getPosition();
		return { pos.x, pos.y, pos.z, 1.0f };
	}

	DirectX::XMFLOAT4 Light::getDirection()
	{
		// Set the out parameter to the direction
		DirectX::XMFLOAT3 fwd = mTransform->getForward();
		return { fwd.x, fwd.y, fwd.z, 0.0f };
	}

	void Light::addColor(DirectX::XMFLOAT3 color)
	{
		// Add to the color while clamping the color between 0 and 1
		mColor.x = min(0.0f, max(mColor.x + color.x, 1.0f));
		mColor.y = min(0.0f, max(mColor.y + color.y, 1.0f));
		mColor.z = min(0.0f, max(mColor.z + color.z, 1.0f));
		mColor.w = 1.0f;
	}

	void Light::addColor(float r, float g, float b)
	{
		// Add to the color while clamping the color between 0 and 1
		mColor.x = min(0.0f, max(mColor.x + r, 1.0f));
		mColor.y = min(0.0f, max(mColor.y + g, 1.0f));
		mColor.z = min(0.0f, max(mColor.z + b, 1.0f));
		mColor.w = 1.0f;
	}

	void Light::setColor(DirectX::XMFLOAT3 color)
	{
		// Set the new color
		mColor = DirectX::XMFLOAT4(color.x, color.y, color.z, 1.0f);
	}

	void Light::setColor(float r, float g, float b)
	{
		// Set the new color
		mColor = DirectX::XMFLOAT4(r, g, b, 1.0f);
	}

	DirectX::XMFLOAT4 Light::getColor()
	{
		// Set the out parameter to the color
		return DirectX::XMFLOAT4(mColor.x, mColor.y, mColor.z, 1.0f);
	}

	void Light::setLightType(LightType lightType)
	{
		// Set the light type
		mLightType = lightType;
	}

	LightType Light::getLightType()
	{
		// Return the light type
		return mLightType;
	}

	void Light::addIntensity(float intensity)
	{
		// Add to the intensity
		mIntensity += intensity;
	}

	void Light::setIntensity(float intensity)
	{
		// Set the new intensity
		mIntensity = intensity;
	}

	float Light::getIntensity()
	{
		// Return the intensity
		return mIntensity;
	}

	void Light::addRange(float range)
	{
		// Add to the range
		mRange += range;
	}

	void Light::setRange(float range)
	{
		// Set the new range
		mRange = range;
	}

	float Light::getRange()
	{
		// Return the range
		return mRange;
	}

	void Light::addSpotAngle(float angle)
	{
		// Add to the spot angle
		mSpotAngle += angle;
	}

	void Light::setSpotAngle(float angle)
	{
		// Set the new spot angle
		mSpotAngle = angle;
	}

	float Light::getSpotAngle()
	{
		// Return the spot angle
		return mSpotAngle;
	}
}