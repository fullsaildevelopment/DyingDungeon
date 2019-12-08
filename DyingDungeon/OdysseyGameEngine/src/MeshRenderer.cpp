#include "MeshRenderer.h"
#include <fstream>
#include "MaterialManager.h"
#include "MeshManager.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, MeshRenderer)

	MeshRenderer::MeshRenderer()
	{
		mDebugEnabled = false;
		mIsActive = false;
	}

	MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> meshID, std::shared_ptr<Material> material)
	{
		mIsActive = true;
		mMesh = meshID;
		mMaterial = material;
	}

	void MeshRenderer::initialize(GameObject* parent)
	{
		mGameObject = parent;
		onEnable();
	}

	void MeshRenderer::setActive(bool active)
	{
		mIsActive = active;
	}

	void MeshRenderer::setMesh(std::shared_ptr<Mesh> mesh)
	{
		mMesh = mesh;
	}

	void MeshRenderer::setMaterial(std::shared_ptr<Material> material)
	{
		mMaterial = material;
	}

	void MeshRenderer::setDebugEnabled(bool enabled)
	{
		mDebugEnabled = enabled;
	}

	bool MeshRenderer::getActive()
	{
		return mIsActive;
	}

	bool MeshRenderer::getDebugEnabled()
	{
		return mDebugEnabled;
	}

	Mesh* MeshRenderer::getMesh()
	{
		return mMesh.get();
	}

	Material* MeshRenderer::getMaterial()
	{
		return mMaterial.get();
	}

	void MeshRenderer::bind()
	{
		mMesh->bind();
		mMaterial->bind();
	}

	void MeshRenderer::unbind()
	{
		mMesh->unbind();
		mMaterial->unbind();
	}
}