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
	class AABB : Component
	{
	public:
		AABB() = default;
		AABB(DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max);
		virtual void initialize(SceneObject* parent);
		void debugDraw(DirectX::XMFLOAT3 color);
		DirectX::XMFLOAT3 calculateCenter();
		DirectX::XMFLOAT3 calculateExtents();
		void calculateAABBFromMesh(DirectX::XMMATRIX worldMatrix, std::vector<Vertex> vertexList);
		bool testAABBtoSphere(Sphere toTest);
	public:
		DirectX::XMFLOAT3 mMin;
		DirectX::XMFLOAT3 mMax;
	};
}