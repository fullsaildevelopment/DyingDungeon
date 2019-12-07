#include "MeshRenderer.h"
#include <fstream>
#include "MaterialManager.h"
#include "MeshManager.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, MeshRenderer)

	MeshRenderer::MeshRenderer(DirectX::XMFLOAT4X4 worldMatrix)
	{
		mDebugEnabled = false;
		mIsActive = false;
		mWorldMatrix = worldMatrix;
	}

	MeshRenderer::MeshRenderer(DirectX::XMFLOAT4X4 worldMatrix, std::shared_ptr<Mesh> meshID, std::shared_ptr<Material> material)
	{
		mIsActive = true;
		mWorldMatrix = worldMatrix;
		mMesh = meshID;
		mMaterial = material;
	}

	void MeshRenderer::initialize(SceneObject* parent)
	{
		onEnable();
		mParent = parent;
	}

	void MeshRenderer::importMesh(const char* filename)
	{
		mIsActive = true;

		// Open the mesh file
		std::fstream file{ filename, std::ios_base::in | std::ios_base::binary };

		// Check if the file is open
		assert(file.is_open());

		// Read in the number of meshes
		uint64_t numMeshes;
		file.read((char*)&numMeshes, sizeof(uint64_t));

		// Read in the world matrix
		file.read((char*)&mWorldMatrix, sizeof(DirectX::XMFLOAT4X4));

		// Send the mesh loading to the mesh manager
		mMesh = MeshManager::getInstance().importMesh(file);
		mMaterial = MaterialManager::getInstance().importMaterial(file);
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

	int MeshRenderer::getMeshID()
	{
		//return mMeshID;
		return 0;
	}

	Material* MeshRenderer::getMaterial()
	{
		return mMaterial.get();
	}

	int MeshRenderer::getMaterialID()
	{
		return 0;
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