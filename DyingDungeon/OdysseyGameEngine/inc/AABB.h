#pragma once
#include "EngineIncludes.h"
#include "Component.h"

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
		AABB(DirectX::XMFLOAT4X4& transform, std::vector<Vertex> vertexList);
		virtual void initialize(GameObject* parent);
		void debugDraw(DirectX::XMFLOAT3 color);
		DirectX::XMFLOAT3 calculateCenter();
		DirectX::XMFLOAT3 calculateExtents();
		bool testAABBtoSphere(Sphere toTest);
	public:
		DirectX::XMFLOAT3 mMin;
		DirectX::XMFLOAT3 mMax;
	};
}