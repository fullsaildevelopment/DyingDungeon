#include "DebugManager.h"
#include "AABB.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, AABB)

	AABB::AABB(DirectX::XMFLOAT4X4& transform, std::vector<Vertex> vertexList)
	{
		// Set he min
		DirectX::XMFLOAT3 min = DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
		DirectX::XMFLOAT3 max = DirectX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (Vertex vertex : vertexList)
		{
			// Convert the vertex to world space
			DirectX::XMFLOAT3 pos = vertex.position;
			DirectX::XMStoreFloat3(&pos, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&vertex.position), DirectX::XMLoadFloat4x4(&transform)));

			// Find the min between the position and the current min
			min.x = (pos.x < min.x) ? pos.x : min.x;
			min.y = (pos.y < min.y) ? pos.y : min.y;
			min.z = (pos.z < min.z) ? pos.z : min.z;

			// Find the max between the position and the current max
			max.x = (pos.x > max.x) ? pos.x : max.x;
			max.y = (pos.y > max.y) ? pos.y : max.y;
			max.z = (pos.z > max.z) ? pos.z : max.z;
		}

		// Assign the new min and max positions
		mMin = min;
		mMax = max;
	}

	void AABB::calculateCenter(DirectX::XMFLOAT3& center)
	{
		// Convert the min and max to a vector
		DirectX::XMVECTOR min = { mMin.x, mMin.y, mMin.z, 0.0f };
		DirectX::XMVECTOR max = { mMax.x, mMax.y, mMax.z, 0.0f };

		// Calculate the center position and store it in the center variable
		DirectX::XMStoreFloat3(&center, DirectX::XMVectorScale(DirectX::XMVectorAdd(max , min), 0.5f));
	}

	void AABB::calculateExtents(DirectX::XMFLOAT3& extents)
	{
		// Convert the min and max to a vector
		DirectX::XMVECTOR min = { mMin.x, mMin.y, mMin.z, 0.0f };
		DirectX::XMVECTOR max = { mMax.x, mMax.y, mMax.z, 0.0f };

		// Calulcate the extents and store it in the extents variable
		XMStoreFloat3(&extents, DirectX::XMVectorScale(DirectX::XMVectorSubtract(max, min), 0.5f));
	}

	bool AABB::testAABBtoSphere(Sphere toTest)
	{
		// Start at the sphere's center point
		DirectX::XMFLOAT3 closestPoint = toTest.center;

		// Clamp the sphere's center to the min and max of the AABB
		closestPoint.x = min(max(toTest.center.x, mMin.x), mMax.x);
		closestPoint.y = min(max(toTest.center.y, mMin.y), mMax.y);
		closestPoint.z = min(max(toTest.center.z, mMin.z), mMax.z);

		// Calculate the squared radius of the sphere
		float sqRadius = toTest.radius * toTest.radius;

		// Calculate the squared distance from the closest point to the sphere's center
		float sqDist = pow(toTest.center.x - closestPoint.x, 2) + pow(toTest.center.y - closestPoint.y, 2) + pow(toTest.center.z - closestPoint.z, 2);

		// Check if the two are colliding
		return (sqDist < sqRadius);
	}

	DirectX::XMFLOAT3 AABB::getMin()
	{
		// Return the min position
		return mMin;
	}

	DirectX::XMFLOAT3 AABB::getMax()
	{
		// Return the max position
		return mMax;
	}

	void AABB::debugDraw(DirectX::XMFLOAT3 color)
	{
		// Calculate the extents
		DirectX::XMFLOAT3 extents;
		calculateExtents(extents);

		// Calculate the center position
		DirectX::XMFLOAT3 center;
		calculateCenter(center);

		// Debug draw the AABB in the debug renderer
		DebugManager::getInstance().addAABB(extents, center, color);
	}
}