#include "MeshRenderer.h"
#include <fstream>
#include "Material.h"
#include "Mesh.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, MeshRenderer)

	MeshRenderer::MeshRenderer()
	{
		// Set default values
		mDebugEnabled = false;
		mIsActive = false;
	}

	MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> meshID, std::shared_ptr<Material> material)
	{
		// The MeshRenderer is now active
		mIsActive = true;

		// Store the new Mesh and Material
		mMesh = meshID;
		mMaterial = material;
	}

	void MeshRenderer::initialize()
	{
		onEnable();
	}

	void MeshRenderer::setActive(bool active)
	{
		// Set the new active state
		mIsActive = active;
	}

	void MeshRenderer::setMesh(std::shared_ptr<Mesh> mesh)
	{
		// Set the new Mesh
		mMesh = mesh;
	}

	void MeshRenderer::setMaterial(std::shared_ptr<Material> material)
	{
		// Set the new Material
		mMaterial = material;
	}

	void MeshRenderer::setDebugEnabled(bool enabled)
	{
		// Set the new debug state
		mDebugEnabled = enabled;
	}

	bool MeshRenderer::getActive()
	{
		// Return the new active state
		return mIsActive;
	}

	bool MeshRenderer::getDebugEnabled()
	{
		// Return the new debug state
		return mDebugEnabled;
	}

	Mesh* MeshRenderer::getMesh()
	{
		// Return a raw pointer to the Mesh
		return mMesh.get();
	}

	Material* MeshRenderer::getMaterial()
	{
		// Return a raw pointer to the Material
		return mMaterial.get();
	}

	void MeshRenderer::bind()
	{
		// Bind the mesh and material to the rendering pipeline
		mMesh->bind();
		mMaterial->bind();
	}

	void MeshRenderer::unbind()
	{
		// Unbind the mesh and material from the rendering pipeline
		mMesh->unbind();
		mMaterial->unbind();
	}
}