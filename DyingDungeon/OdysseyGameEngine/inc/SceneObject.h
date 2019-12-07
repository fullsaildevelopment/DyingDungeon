#pragma once
#include "EngineIncludes.h"
#include "Animator.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "AABB.h"

namespace Odyssey
{
	class SceneObject
	{
	public:
		SceneObject();
	public:
		void importModel(const char* filename);
		void addPosition(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setScale(float x, float y, float z);
	public: // Components
		void attachAnimator();
		void attachMeshRenderer();
		void attachMeshRenderer(std::shared_ptr<Mesh> meshID, std::shared_ptr<Material> materialID);
		void attachParticleSystem();
		void attachAABB();
		Animator* getAnimator();
		Animator* getRootAnimator();
		MeshRenderer* getMeshRenderer();
		ParticleSystem* getParticleSystem();
		ParticleSystem* getRootParticleSystem();
		AABB* getAABB();
		bool hasAnimator();
		bool hasMeshRenderer();
		bool hasParticleSystem();
		bool hasAABB();
	public: // Accessors
		void getLocalTransform(DirectX::XMFLOAT4X4& localTransform);
		void getGlobalTransform(DirectX::XMFLOAT4X4& globalTransform);
		const std::vector<std::shared_ptr<SceneObject>> getChildren();
		const int getChildrenCount();
	public: //Debug
		void enableDebug();
		void disableDebug();
		bool getDebugEnabled();
	private:
		DirectX::XMFLOAT4X4 mWorldMatrix;
		std::vector<std::shared_ptr<SceneObject>> children;
		SceneObject* mParent;
		bool mDebugEnabled;
		// Components
		std::unique_ptr<Animator> mAnimator;
		std::unique_ptr<MeshRenderer> mMeshRenderer;
		std::unique_ptr<ParticleSystem> mParticleSystem;
		std::unique_ptr<AABB> mAABB;
	};
}