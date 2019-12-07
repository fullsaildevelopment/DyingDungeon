#pragma once
#include "EngineIncludes.h"
#include "Component.h"

namespace Odyssey
{
	class Mesh;
	class Material;
	class RenderState;

	class MeshRenderer : public Component
	{
		CLASS_DECLARATION(Animator)

	public: // Constructors
		MeshRenderer(DirectX::XMFLOAT4X4 worldMatrix);
		MeshRenderer(DirectX::XMFLOAT4X4 worldMatrix, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
	public: // Component Inherited Functions
		virtual void initialize(SceneObject* parent);
	public: // Functions
		void importMesh(const char* filename);
	public: // Mutators
		void setActive(bool active);
		void setMesh(std::shared_ptr<Mesh> mesh);
		void setMaterial(std::shared_ptr<Material> material);
		void setDebugEnabled(bool enabled);
	public: // Accessors
		bool getActive();
		bool getDebugEnabled();
		Mesh* getMesh();
		int getMeshID();
		Material* getMaterial();
		int getMaterialID();
	public: // Rendering Functions
		void bind();
		void unbind();
	private:
		DirectX::XMFLOAT4X4 mWorldMatrix;
		bool mIsActive;
		bool mDebugEnabled;
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;
	};
}