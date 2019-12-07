#include "AABB.h"
#include "DebugManager.h"

namespace Odyssey
{
	AABB::AABB(DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max)
	{
		mMin = min;
		mMax = max;
	}

	DirectX::XMFLOAT3 AABB::calculateCenter()
	{
		using namespace DirectX;

		// Convert the min and max to a vector
		XMVECTOR min = { mMin.x, mMin.y, mMin.z, 0.0f };
		XMVECTOR max = { mMax.x, mMax.y, mMax.z, 0.0f };

		// Calulcate the center and return it
		XMFLOAT3 center;
		XMStoreFloat3(&center, (max + min) / 2.0f);
		return center;
	}

	DirectX::XMFLOAT3 AABB::calculateExtents()
	{
		using namespace DirectX;

		// Convert the min and max to a vector
		XMVECTOR min = { mMin.x, mMin.y, mMin.z, 0.0f };
		XMVECTOR max = { mMax.x, mMax.y, mMax.z, 0.0f };

		// Calulcate the extents and return it
		XMFLOAT3 extents;
		XMStoreFloat3(&extents, (max - min) / 2.0f);
		return extents;
	}

	void AABB::calculateAABBFromMesh(DirectX::XMMATRIX worldMatrix, std::vector<Vertex> vertexList)
	{
		float minX = FLT_MAX;
		float minY = FLT_MAX;
		float minZ = FLT_MAX;

		float maxX = -FLT_MAX;
		float maxY = -FLT_MAX;
		float maxZ = -FLT_MAX;

		for (Vertex vertex : vertexList)
		{
			// Convert the vertex to world space
			DirectX::XMFLOAT3 pos = vertex.position;
			DirectX::XMStoreFloat3(&pos, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&vertex.position), worldMatrix));
			minX = (pos.x < minX) ? pos.x : minX;
			minY = (pos.y < minY) ? pos.y : minY;
			minZ = (pos.z < minZ) ? pos.z : minZ;

			maxX = (pos.x > maxX) ? pos.x : maxX;
			maxY = (pos.y > maxY) ? pos.y : maxY;
			maxZ = (pos.z > maxZ) ? pos.z : maxZ;
		}

		mMin.x = minX;
		mMin.y = minY;
		mMin.z = minZ;

		mMax.x = maxX;
		mMax.y = maxY;
		mMax.z = maxZ;

		if (mMax.y > 100.0f)
		{
			int debug = 0;
		}
	}

	bool AABB::testAABBtoSphere(Sphere toTest)
	{
		using namespace DirectX;
		XMFLOAT3 closestPoint = toTest.center;

		closestPoint.x = min(max(toTest.center.x, mMin.x), mMax.x);
		closestPoint.y = min(max(toTest.center.y, mMin.y), mMax.y);
		closestPoint.z = min(max(toTest.center.z, mMin.z), mMax.z);

		float sqRadius = toTest.radius * toTest.radius;
		float sqDist = pow(toTest.center.x - closestPoint.x, 2) + pow(toTest.center.y - closestPoint.y, 2) + pow(toTest.center.z - closestPoint.z, 2);
		return (sqDist < sqRadius);
	}

	void AABB::initialize(SceneObject* parent)
	{
		mParent = parent;
		mMin = { 0,0,0 };
		mMax = { 0,0,0 };
	}

	void AABB::debugDraw(DirectX::XMFLOAT3 color)
	{
		DirectX::XMFLOAT3 extents = calculateExtents();
		DirectX::XMFLOAT3 center = calculateCenter();
		DebugManager::getInstance().addAABB(extents, center, color);
	}
}