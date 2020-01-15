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
		mActive = false;
	}

	MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> meshID, std::shared_ptr<Material> material)
	{
		// The MeshRenderer is now active
		mActive = true;

		// Store the new Mesh and Material
		mMesh = meshID;
		mMaterial = material;
	}

	void MeshRenderer::setMesh(std::shared_ptr<Mesh> mesh)
	{
		// Set the new Mesh
		mLock.lock(LockState::Write);
		mMesh = mesh;
		mLock.unlock(LockState::Write);
	}

	void MeshRenderer::setMaterial(std::shared_ptr<Material> material)
	{
		// Set the new Material
		mLock.lock(LockState::Write);
		mMaterial = material;
		mLock.unlock(LockState::Write);
	}

	void MeshRenderer::setDebugEnabled(bool enabled)
	{
		// Set the new debug state
		mLock.lock(LockState::Write);
		mDebugEnabled = enabled;
		mLock.unlock(LockState::Write);
	}

	bool MeshRenderer::getDebugEnabled()
	{
		// Return the new debug state
		mLock.lock(LockState::Read);
		bool debug = mDebugEnabled;
		mLock.unlock(LockState::Read);

		return debug;
	}

	Mesh* MeshRenderer::getMesh()
	{
		// Return a raw pointer to the Mesh
		mLock.lock(LockState::Read);
		Mesh* mesh = mMesh.get();
		mLock.unlock(LockState::Read);

		return mesh;
	}

	Material* MeshRenderer::getMaterial()
	{
		// Return a raw pointer to the Material
		mLock.lock(LockState::Read);
		Material* material = mMaterial.get();
		mLock.unlock(LockState::Read);

		return material;
	}

	void MeshRenderer::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Bind the mesh and material to the rendering pipeline
		mMesh->bind(context);
		mMaterial->bind(context);
	}

	void MeshRenderer::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Unbind the mesh and material from the rendering pipeline
		mMesh->unbind(context);
		mMaterial->unbind(context);
	}
}