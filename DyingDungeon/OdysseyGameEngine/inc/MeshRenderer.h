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
		MeshRenderer();
		MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
	public: // Component Inherited Functions
		virtual void initialize();
	public: // Mutators
		void setActive(bool active);
		void setMesh(std::shared_ptr<Mesh> mesh);
		void setMaterial(std::shared_ptr<Material> material);
		void setDebugEnabled(bool enabled);
	public: // Accessors
		bool getActive();
		bool getDebugEnabled();
		Mesh* getMesh();
		Material* getMaterial();
	public: // Rendering Functions
		void bind();
		void unbind();
	private:
		bool mIsActive;
		bool mDebugEnabled;
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;
	};
}