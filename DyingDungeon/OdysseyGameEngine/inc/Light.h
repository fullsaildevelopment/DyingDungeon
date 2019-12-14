#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	enum class LightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2
	};

	class Light
	{
	public: // Constructors
		Light() = default;
	public: // Public functions
		void debugDraw();
		DirectX::XMFLOAT4X4 buildLightTransform(float sceneRadius, DirectX::XMFLOAT3 sceneCenter);
		DirectX::XMFLOAT4X4 buildLightTransformProjection(float sceneRadius, DirectX::XMFLOAT3 sceneCenter);
		void addRange(float range);
		void setPosition(float x, float y, float z);
		void addPosition(float x, float y, float z);
		void setDirection(float x, float y, float z);
		void addDirection(float x, float y, float z);
		void rotateDirectionX(float angleInDegrees);
		void rotateDirectionY(float angleInDegrees);
		void rotateDirectionZ(float angleInDegrees);
		DirectX::XMFLOAT3 getPosition();
	public:
		DirectX::XMFLOAT4 mWorldPosition;
		DirectX::XMFLOAT4 mWorldDirection;
		DirectX::XMFLOAT4 mColor;
		LightType mLightType;
		float mIntensity;
		float mRange;
		float mSpotAngle;
	};
}