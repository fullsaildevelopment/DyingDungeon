#pragma once
#include "EngineIncludes.h"
#include "Component.h"

namespace Odyssey
{
	class Transform : public Component
	{
		CLASS_DECLARATION(Transform)

	public: // Rule of 3
		Transform();
		Transform(DirectX::XMFLOAT4X4 worldMatrix);

	public: // Interface
		/**
		 *	Add to the transform's position.
		 *	@param[in] x The x value to add to the position.
		 *	@param[in] y The y value to add to the position.
		 *	@param[in] z The z value to add to the position.
		 *	@return void
		 */
		void addPosition(float x, float y, float z);

		/**
		 *	Set the transform's position.
		 *	@param[in] x The x value of the position.
		 *	@param[in] y The y value of the position.
		 *	@param[in] z The z value of the position.
		 *	@return void
		 */
		void setPosition(float x, float y, float z);

		/**
		 *	Get the transform's position.
		 *	@param[in] void
		 *	@return XMFLOAT3 The transform's position.
		 */
		DirectX::XMFLOAT3 getPosition();

		/**
		 *	Add to the transform's euler rotation.
		 *	@param[in] x The x value to add to the rotation.
		 *	@param[in] y The y value to add to the rotation.
		 *	@param[in] z The z value to add to the rotation.
		 *	@return void
		 */
		void addRotation(float x, float y, float z);

		/**
		 *	Set the transform's euler rotation.
		 *	@param[in] x The x value of the rotation.
		 *	@param[in] y The y value of the rotation.
		 *	@param[in] z The z value of the rotation.
		 *	@return void
		 */
		void setRotation(float x, float y, float z);

		/**
		 *	Get the transform's euler rotation.
		 *	@param[in] void 
		 *	@return XMFLOAT3 The transform's euler rotation.
		 */
		DirectX::XMFLOAT3 getEulerRotation();

		/**
		 *	Add to the transform's scale.
		 *	@param[in] x The x value to add to the scale.
		 *	@param[in] y The y value to add to the scale.
		 *	@param[in] z The z value to add to the scale.
		 *	@return void
		 */
		void addScale(float x, float y, float z);

		/**
		 *	Set the transform's scale.
		 *	@param[in] x The x value of the scale.
		 *	@param[in] y The y value of the scale.
		 *	@param[in] z The z value of the scale.
		 *	@return void
		 */
		void setScale(float x, float y, float z);

		/**
		 *	Get the transform's scale.
		 *	@param[in] void 
		 *	@return XMFLOAT3 The transform's scale.
		 */
		DirectX::XMFLOAT3 getScale();

		/**
		 *	Get the transform's forward direction.
		 *	@param[in] void
		 *	@return XMFLOAT3 The transform's forward direction.
		 */
		DirectX::XMFLOAT3 getForward();

		/**
		 *	Get the transform's right direction.
		 *	@param[in] void 
		 *	@return XMFLOAT3 The transform's right direction.
		 */
		DirectX::XMFLOAT3 getRight();

		/**
		 *	Get the transform's up direction.
		 *	@param[in] void 
		 *	@return XMFLOAT3 The transform's up direction.
		 */
		DirectX::XMFLOAT3 getUp();

		/**
		 *	Get the transform's local-space matrix.
		 *	@param[out] void
		 *	@return XMFLOAT4X4 The transform's local-space matrix.
		 */
		DirectX::XMFLOAT4X4 getLocalTransform(bool ignoreScale = false);

		/**
		 *	Get the transform's global-space matrix.
		 *	@param[out] void 
		 *	@return XMFLOAT4X4 The transform's global-space matrix.
		 */
		DirectX::XMFLOAT4X4 getGlobalTransform(bool ignoreScale = false);

	private: // Helpers
		/**
		 *	Decompose the world matrix to determine the euler angle rotation.
		 *	@param[in] void
		 *	@return void
		 */
		void calculateEulerRotations();

		/**
		 *	Composes the position, rotation and scale into a world matrix.
		 *	@param[in] void
		 *	@return void
		 */
		void recalculateWorldMatrix();

	private: // Members
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mRotation;
		DirectX::XMFLOAT3 mScale;
		DirectX::XMFLOAT4X4 mWorldMatrix;
	};
}