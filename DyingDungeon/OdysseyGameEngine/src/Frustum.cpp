#include "Frustum.h"

namespace Odyssey
{
	Plane Frustum::calculatePlane(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c, DirectX::XMFLOAT3 d)
	{
		Plane plane;
		DirectX::XMFLOAT3 c1 = DirectX::XMFLOAT3(b.x - a.x, b.y - a.y, b.z - a.z);
		DirectX::XMFLOAT3 c2 = DirectX::XMFLOAT3(c.x - a.x, c.y - a.y, c.z - a.z);
		DirectX::XMVECTOR vNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&c1), DirectX::XMLoadFloat3(&c2)));
		DirectX::XMStoreFloat3(&plane.normal, vNormal);
		DirectX::XMVECTOR vOffset = DirectX::XMVector3Dot(vNormal, DirectX::XMLoadFloat3(&a));
		plane.offset = DirectX::XMVectorGetX(vOffset);
		DirectX::XMFLOAT3 center;
		center.x = (a.x + b.x + c.x + d.x) / 4.0f;
		center.y = (a.y + b.y + c.y + d.y) / 4.0f;
		center.z = (a.z + b.z + c.z + d.z) / 4.0f;

		return plane;
	}

	int Frustum::classifySphereToPlane(Sphere& sphere, Plane& plane)
	{
		using namespace DirectX;
		// Signed Distance = center point dot plane normal
		float distanceFromOrigin = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&sphere.center), (XMLoadFloat3(&plane.normal))));

		// Distance From Plane = Signed Distance - offset
		float distanceFromPlane = distanceFromOrigin - plane.offset;
		if (distanceFromPlane > sphere.radius)
		{
			// if (distance from plane > r) = sphere in front
			return 1;
		}
		else if (distanceFromPlane < (-sphere.radius))
		{
			// if (distance from plane < -r) sphere behind
			return -1;
		}
		else
		{
			// Otherwise sphere overlapping
			return 0;
		}
	}
	int Frustum::classifyAABBToPlane(AABB& aabb, Plane& plane)
	{
		Sphere aabbSphere;
		aabbSphere.center = aabb.calculateCenter();
		// Radius = aabb.extents dot abs(plane.normal)
		aabbSphere.radius = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&aabb.calculateExtents()), DirectX::XMVectorAbs(XMLoadFloat3(&plane.normal))));

		return classifySphereToPlane(aabbSphere, plane);
	}
	bool Frustum::checkFrustumView(AABB& aabb)
	{
		for (int i = 0; i < 6; i++)
		{
			if (classifyAABBToPlane(aabb, mFrustumPlanes[i]) == -1)
			{
				return false;
			}
		}
		return true;
	}

	void Frustum::calculateFrustum(float fov, float aspectRatio, float nearPlane, float farPlane, DirectX::XMFLOAT4X4 view)
	{
		using namespace DirectX;
		float heightNear = 2.0f * tan(DirectX::XMConvertToRadians(fov) / 2.0f) * nearPlane;
		float heightFar = 2.0f * tan(DirectX::XMConvertToRadians(fov) / 2.0f) * farPlane;
		float widthNear = heightNear * aspectRatio;
		float widthFar = heightFar * aspectRatio;

		XMVECTOR camPos = XMVectorSet(view.m[3][0], view.m[3][1], view.m[3][2], 0.0f);
		XMVECTOR camX = XMVectorSet(view.m[0][0], view.m[0][1], view.m[0][2], 0.0f);
		XMVECTOR camY = XMVectorSet(view.m[1][0], view.m[1][1], view.m[1][2], 0.0f);
		XMVECTOR camZ = XMVectorSet(view.m[2][0], view.m[2][1], view.m[2][2], 0.0f);

		XMVECTOR nearCenterVector = camPos + (camZ * nearPlane);
		XMVECTOR farCenterVector = camPos + (camZ * farPlane);
		XMFLOAT3 frustumCoords[8];

		// NEAR TOP LEFT
		XMVECTOR temp = nearCenterVector + XMVectorScale(camY, heightNear * 0.5f) - XMVectorScale(camX, widthNear * 0.5f);
		XMStoreFloat3(&frustumCoords[0], temp);
		// NEAR TOP RIGHT
		temp = nearCenterVector + XMVectorScale(camY, heightNear * 0.5f) + XMVectorScale(camX, widthNear * 0.5f);
		XMStoreFloat3(&frustumCoords[1], temp);
		// NEAR BOTTOM RIGHT
		temp = nearCenterVector - XMVectorScale(camY, heightNear * 0.5f) + XMVectorScale(camX, widthNear * 0.5f);
		XMStoreFloat3(&frustumCoords[2], temp);
		// NEAR BOTTOM LEFT
		temp = nearCenterVector - XMVectorScale(camY, heightNear * 0.5f) - XMVectorScale(camX, widthNear * 0.5f);
		XMStoreFloat3(&frustumCoords[3], temp);

		// FAR TOP LEFT
		temp = farCenterVector + XMVectorScale(camY, heightFar * 0.5f) - XMVectorScale(camX, widthFar * 0.5f);
		XMStoreFloat3(&frustumCoords[4], temp);
		// FAR TOP RIGHT
		temp = farCenterVector + XMVectorScale(camY, heightFar * 0.5f) + XMVectorScale(camX, widthFar * 0.5f);
		XMStoreFloat3(&frustumCoords[5], temp);
		// FAR BOTTOM RIGHT
		temp = farCenterVector - XMVectorScale(camY, heightFar * 0.5f) + XMVectorScale(camX, widthFar * 0.5f);
		XMStoreFloat3(&frustumCoords[6], temp);
		// FAR BOTTOM LEFT
		temp = farCenterVector - XMVectorScale(camY, heightFar * 0.5f) - XMVectorScale(camX, widthFar * 0.5f);
		XMStoreFloat3(&frustumCoords[7], temp);

		// Near Plane
		mFrustumPlanes[0] = calculatePlane(frustumCoords[1], frustumCoords[0], frustumCoords[3], frustumCoords[2]);
		// Left Plane
		mFrustumPlanes[1] = calculatePlane(frustumCoords[0], frustumCoords[4], frustumCoords[7], frustumCoords[3]);
		// Right Plane
		mFrustumPlanes[2] = calculatePlane(frustumCoords[5], frustumCoords[1], frustumCoords[2], frustumCoords[6]);
		// Far Plane
		mFrustumPlanes[3] = calculatePlane(frustumCoords[4], frustumCoords[5], frustumCoords[6], frustumCoords[7]);
		// Top Plane
		mFrustumPlanes[4] = calculatePlane(frustumCoords[5], frustumCoords[4], frustumCoords[0], frustumCoords[1]);
		// Bot Plane
		mFrustumPlanes[5] = calculatePlane(frustumCoords[2], frustumCoords[3], frustumCoords[7], frustumCoords[6]);
	}
}