#pragma once
#include "AABB.h"

namespace Odyssey
{
	struct Plane
	{
		DirectX::XMFLOAT3 normal;
		float offset;
	};

	class AABB;
	class Frustum
	{
	private:
		Plane calculatePlane(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c, DirectX::XMFLOAT3 d);
		int classifySphereToPlane(Sphere& sphere, Plane& plane);
		int classifyAABBToPlane(AABB& aabb, Plane& plane);
	public:
		bool checkFrustumView(AABB& aabb);
		void calculateFrustum(float fov, float aspectRatio, float nearPlane, float farPlane, DirectX::XMFLOAT4X4 view);
	private:
		std::array<Plane, 6> mFrustumPlanes;
	};
}