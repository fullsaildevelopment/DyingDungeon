#pragma once
#include "EngineIncludes.h"
#include "Component.h"
/***********************************************
 * Filename:          AABB.h
 * Date:              12/04/2019
 * Mod. Date:         12/14/2019
 * Author:            Max Nastri
 * Purpose:           Properties and logic for an
					  Axis Aligned Bounding Box.
 ************************************************/
namespace Odyssey
{
	struct Sphere
	{
		DirectX::XMFLOAT3 center;
		float radius;
	};

	class AABB : public Component
	{
		CLASS_DECLARATION(AABB)
	public:
		/**
		 *	Construct an AABB based on the input vertices transformed by the input world matrix.
		 *	@param[in] transform: World-Space position in which to evaluate the vertex list.
		 *	@param[in] vertexList: Container of vertices to base the AABB calculations
		 */
		AABB(DirectX::XMFLOAT4X4& transform, std::vector<Vertex> vertexList);

		/**
		 *	Initialize the AABB component with default values. Called once when the scene is loaded.
		 *	@param[in] void
		 *	@return void
		 */
		virtual void initialize();

		/**
		 *	Draw the AABB component in wireframe using the debug renderer.
		 *	@param[in] color The color used to render the AABB.
		 *	@return void
		 */
		void debugDraw(DirectX::XMFLOAT3 color);

		/**
		 *	Calculate the center position of the AABB component.
		 *	@param[out] center The container variable for the calculated center.
		 *	@return void
		 */
		void calculateCenter(DirectX::XMFLOAT3& center);

		/**
		 *	Calculate the extents of the AABB component.
		 *	@param[out] center The container variable for the calculated center.
		 *	@return void
		 */
		void calculateExtents(DirectX::XMFLOAT3& extents);

		/**
		 *	Test the AABB component against a Sphere object for collision.
		 *	@param[in] toTest The Sphere object to test against the AABB.
		 *	@return bool Returns whether the two objects are collider or not.
		 */
		bool testAABBtoSphere(Sphere toTest);

		/**
		 *	Get the min position of the AABB component
		 *	@param[in] void
		 *	@return XMFloat3 Returns the min position in XMFloat3 format.
		 */
		DirectX::XMFLOAT3 getMin();

		/**
		 *	Get the max position of the AABB component
		 *	@param[in] void
		 *	@return XMFloat3 Returns the max position in XMFloat3 format.
		 */
		DirectX::XMFLOAT3 getMax();
	private:
		DirectX::XMFLOAT3 mMin;
		DirectX::XMFLOAT3 mMax;
	};
}