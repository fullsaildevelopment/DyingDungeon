#pragma once
#include "EngineIncludes.h"
#include "Component.h"

namespace Odyssey
{
	class Frustum;

	class Camera : public Component
	{
		CLASS_DECLARATION(Camera)
	public:
		Camera();
	public:
		virtual void initialize(GameObject* gameObject);
	public: // Mutators
		void setFieldOfView(float angleInDegrees);
		void setAspectRatio(float aspectRatio);
		void setNearPlane(float nearPlane);
		void setFarPlane(float farPlane);
		Frustum* getFrustum();
		const DirectX::XMFLOAT4X4 getInverseViewMatrix();
	public: // Accessors
		void setProjectionValues(float fov, float aspectRatio, float nearPlane, float farPlane);
		const DirectX::XMFLOAT4X4& getProjectionMatrix();
		// Frustum Stuff
		std::shared_ptr<Frustum> mFrustum;
	private:
		void buildProjectionMatrix();
	private:
		DirectX::XMFLOAT4X4 mProjMatrix;
		// Camera properties
		float mFieldOfView;
		float mNear;
		float mFar;
		float mAspectRatio;
	};
}