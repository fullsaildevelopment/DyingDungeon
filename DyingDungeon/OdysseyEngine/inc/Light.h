#pragma once
#include "EngineIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class Light
	{
	public: // Rule of 3
		Light() = default;

	public: // Interface
		/**
		 *	Draw the Light in wireframe mode using the debug renderer.
		 *	@param[in] void
		 *	@return void
		 */
		void debugDraw();

		/**
		 *	Build a view matrix for the Light using the scene center and radius.
		 *	@param[in] sceneRadius The radius of the scene.
		 *	@param[in] sceneCenter The center position of the scene.
		 *	@return XMFloat4x4 The view matrix of the Light in the scene.
		 */
		void buildLightTransform(DirectX::XMFLOAT3& sceneCenter, float sceneRadius, DirectX::XMFLOAT4X4& lightTransform);

		/**
		 *	Build a view * projection matrix for the Light using the scene center and radius.
		 *	@param[in] sceneRadius The radius of the scene.
		 *	@param[in] sceneCenter The center position of the scene.
		 *	@return XMFloat4x4 the view * projection matrix of the Light in the scene.
		 */
		void buildLightTransformProjection(DirectX::XMFLOAT3& sceneCenter, float sceneRadius, DirectX::XMFLOAT4X4& lightTransform);

		/**
		 *	Add to the position of the Light.
		 *	@param[in] x The x value to add to the position.
		 *	@param[in] y The y value to add to the position.
		 *	@param[in] z The z value to add to the position.
		 *	@return void
		 */
		void addPosition(float x, float y, float z);

		/**
		 *	Set the position of the Light in world space.
		 *	@param[in] x The x value to set the position.
		 *	@param[in] y The y value to set the position.
		 *	@param[in] z The z value to set the position.
		 *	@return void
		 */
		void setPosition(float x, float y, float z);

		/**
		 *	Get the position of the Light in world space.
		 *	@param[out] position The position of the light in world space.
		 *	@return void
		 */
		void getPosition(DirectX::XMFLOAT3& position);

		/**
		 *	Add to the direction of the Light.
		 *	@param[in] x The x value to add to the direction.
		 *	@param[in] y The y value to add to the direction.
		 *	@param[in] z The z value to add to the direction.
		 *	@return void
		 */
		void addDirection(float x, float y, float z);

		/**
		 *	Set the direction of the Light in world space.
		 *	@param[in] x The x value to set the direction.
		 *	@param[in] y The y value to set the direction.
		 *	@param[in] z The z value to set the direction.
		 *	@return void
		 */
		void setDirection(float x, float y, float z);

		/**
		 *	Get the direction of the Light in world space.
		 *	@param[out] direction The direction of the light in world space.
		 *	@return void
		 */
		void getDirection(DirectX::XMFLOAT3& direction);

		/**
		 *	Rotate the direction of the Light on the X axis.
		 *	@param[in] angle The angle, in degrees, to rotate the Light by.
		 *	@return void
		 */
		void rotateDirectionX(float angle);

		/**
		 *	Rotate the direction of the Light on the Y axis.
		 *	@param[in] angle The angle, in degrees, to rotate the Light by.
		 *	@return void
		 */
		void rotateDirectionY(float angle);

		/**
		 *	Rotate the direction of the Light on the Z axis.
		 *	@param[in] angle The angle, in degrees, to rotate the Light by.
		 *	@return void
		 */
		void rotateDirectionZ(float angle);

		/**
		 *	Add to the color of the Light.
		 *	@param[in] color The RGB channel value to add to the color.
		 *	@return void
		 */
		void addColor(DirectX::XMFLOAT3 color);

		/**
		 *	Add to the color of the Light.
		 *	@param[in] r The r channel value to add to the color.
		 *	@param[in] g The g channel value to add to the color.
		 *	@param[in] b The b channel value to add to the color.
		 *	@return void
		 */
		void addColor(float r, float g, float b);

		/**
		 *	Set the color of the Light.
		 *	@param[in] color The RGB channels of the color to set.
		 *	@return void
		 */
		void setColor(DirectX::XMFLOAT3 color);

		/**
		 *	Set the color of the Light.
		 *	@param[in] r The r channel value of the color
		 *	@param[in] g The g channel value of the color
		 *	@param[in] b The b channel value of the color
		 *	@return void
		 */
		void setColor(float r, float g, float b);

		/**
		 *	Get the color of the Light.
		 *	@param[out] color The color of the light.
		 *	@return void
		 */
		void getColor(DirectX::XMFLOAT3& color);

		/**
		 *	Set the type of the Light
		 *	@param[in] lightType The type of the light.
		 *	@return void
		 */
		void setLightType(LightType lightType);

		/**
		 *	Set the type of the Light
		 *	@param[in] void
		 *	@return LightType The type of the Light.
		 */
		LightType getLightType();

		/**
		 *	Add to the intensity of the Light
		 *	@param[in] intensity The value to add to the intensity.
		 *	@return void
		 */
		void addIntensity(float intensity);

		/**
		 *	Set the intensity of the Light.
		 *	@param[in] intensity The intensity of the Light.
		 *	@return void
		 */
		void setIntensity(float intensity);

		/**
		 *	Get the intensity of the Light
		 *	@param[in] void
		 *	@return float The intensity of the Light.
		 */
		float getIntensity();

		/**
		 *	Add to the current range of the Light.
		 *	@param[in] range The value to add to the range.
		 *	@return void
		 */
		void addRange(float range);

		/**
		 *	Set the range of the Light.
		 *	@param[in] range The range of the Light.
		 *	@return void
		 */
		void setRange(float range);

		/**
		 *	Add to the current range of the light.
		 *	@param[in] void
		 *	@return float The range of the light.
		 */
		float getRange();

		/**
		 *	Add to the current spot angle of the Light.
		 *	@param[in] range The value to add to the spot angle.
		 *	@return void
		 */
		void addSpotAngle(float angle);

		/**
		 *	Set the spot angle of the Light.
		 *	@param[in] range The spot angle of the Light.
		 *	@return void
		 */
		void setSpotAngle(float angle);

		/**
		 *	Set the spot angle of the Light.
		 *	@param[in] void
		 *	@return float The spot angle of the Light.
		 */
		float getSpotAngle();

	private: // Members
		DirectX::XMFLOAT4 mPosition;
		DirectX::XMFLOAT4 mDirection;
		DirectX::XMFLOAT4 mColor;
		LightType mLightType;
		float mIntensity;
		float mRange;
		float mSpotAngle;
	};
}