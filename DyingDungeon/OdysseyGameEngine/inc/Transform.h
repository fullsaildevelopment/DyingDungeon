#pragma once
#include "EngineIncludes.h"
#include "Component.h"

namespace Odyssey
{
	class Transform : public Component
	{
		CLASS_DECLARATION(Transform)

	public:
		Transform();
		Transform(DirectX::XMFLOAT4X4 worldMatrix);
	public:
		virtual void initialize(SceneObject* parent);
	public: // Mutators
		void addPosition(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void addRotation(float x, float y, float z);
		void setRotation(float x, float y, float z);
		void addScale(float x, float y, float z);
		void setScale(float x, float y, float z);
	public: // Accessors
		DirectX::XMFLOAT3 getPosition();
		DirectX::XMFLOAT3 getRotation();
		DirectX::XMFLOAT3 getScale();
		DirectX::XMFLOAT4X4 getLocalTransform();
		void getGlobalTransform(DirectX::XMFLOAT4X4& globalTransform);
	private:
		void recalculateWorldMatrix();
	private:
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mRotation;
		DirectX::XMFLOAT3 mScale;
		DirectX::XMFLOAT4X4 mWorldMatrix;
	};
}