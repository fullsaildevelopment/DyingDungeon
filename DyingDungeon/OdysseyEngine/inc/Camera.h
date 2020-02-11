#pragma once
#include "Component.h"
#include <DirectXMath.h>

namespace Odyssey
{
	class Frustum;

	class Camera : public Component
	{
		CLASS_DECLARATION(Camera)

	public:
		virtual std::shared_ptr<Component> clone() const;
	public:
		Camera();

	public: // Interface
		virtual void initialize();
		/**
		 *	Set the camera's field of view. 
		 *	@param[in] angleInDegrees The field of view angle in degrees.
		 *	@return void
		 */
		void setFieldOfView(float angleInDegrees);

		/**
		 *	Set the camera's aspect ratio.
		 *	@param[in] aspectRatio The aspect ratio in width / height format.
		 *	@return void
		 */
		void setAspectRatio(float aspectRatio);

		/**
		 *	Set the camera's near plane value.
		 *	@param[in] nearPlane The near plane value.
		 *	@return void
		 */
		void setNearPlane(float nearPlane);

		/**
		 *	Set the camera's far plane value.
		 *	@param[in] farPlane The far plane value.
		 *	@return void
		 */
		void setFarPlane(float farPlane);

		/**
		 *	Get the camera's frustum object.
		 *	@param[in] void
		 *	@return Frustum* A raw pointer to the frustum object.
		 */
		Frustum* getFrustum();

		/**
		 *	Get the camera’s inverse view matrix.
		 *	@param[in] void
		 *	@return XMFLOAT4X4 The camera's inverse view matrix.
		 */
		const DirectX::XMFLOAT4X4 getInverseViewMatrix();

		/**
		 *	Get the camera’s projection matrix.
		 *	@param[in] void
		 *	@return XMFLOAT4X4 The camera's projection matrix.
		 */
		const DirectX::XMFLOAT4X4 getProjectionMatrix();

	private: // Helpers
		/**
		 *	Set the camera's projection values and rebuild the projection matrix.
		 *	@param[in] fov The camera's field of view.
		 *	@param[in] aspectRatio The camera's aspect ratio.
		 *	@param[in] nearPlane The camera's near plane.
		 *	@param[in] farPlane The camera's far plane.
		 *	@return void
		 */
		void setProjectionValues(float fov, float aspectRatio, float nearPlane, float farPlane);

		/**
		 *	Build the projection matrix of the camera.
		 *	@param[in] void
		 *	@return void
		 */
		void buildProjectionMatrix();

	private: // Members
		std::shared_ptr<Frustum> mFrustum;
		DirectX::XMFLOAT4X4 mProjMatrix;
		float mFieldOfView;
		float mNear;
		float mFar;
		float mAspectRatio;
	};
}