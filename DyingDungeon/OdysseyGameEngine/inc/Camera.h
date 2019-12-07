#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class Frustum;

	class Camera
	{
	public:
		Camera();
	public: // Public functions
		// Update the camera to a new position and rotation
		void updateCamera(float x, float y, float z, float pitch, float yaw, float roll);
	public: // Mutators
		void setPosition(float x, float y, float z);
		void setFieldOfView(float angleInDegrees);
		void setAspectRatio(float aspectRatio);
		void setNearPlane(float nearPlane);
		void setFarPlane(float farPlane);
		DirectX::XMFLOAT3 getPosition();
	public: // Accessors
		void setProjectionValues(float fov, float aspectRatio, float nearPlane, float farPlane);
		const DirectX::XMFLOAT4X4& getViewMatrix();
		const DirectX::XMFLOAT4X4& getInverseViewMatrix();
		const DirectX::XMFLOAT4X4& getProjectionMatrix();
		// Frustum Stuff
		std::shared_ptr<Frustum> mFrustum;
	private:
		void buildProjectionMatrix();
	private:
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT4X4 mViewMatrix;
		DirectX::XMFLOAT4X4 mInverseViewMatrix;
		DirectX::XMFLOAT4X4 mProjMatrix;
		// Camera properties
		float mFieldOfView;
		float mNear;
		float mFar;
		float mAspectRatio;
	};
}