#include "Material.h"
#include "Mesh.h"
#include "RenderManager.h"
#include "MeshRenderer.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, MeshRenderer)

	std::shared_ptr<Component> MeshRenderer::clone() const
	{
		return std::make_shared<MeshRenderer>(*this);
	}

	MeshRenderer::MeshRenderer()
	{
		// Set default values
		mDebugEnabled = false;
		mIsActive = false;
		mMaterial = -1;
		mMesh = -1;
		mShadowCaster = true;
	}

	MeshRenderer::MeshRenderer(int meshID, int materialID)
	{
		// Set default values
		mDebugEnabled = false;

		// The MeshRenderer is now active
		mIsActive = true;

		// Store the new Mesh and Material
		mMesh = meshID;
		mMaterial = materialID;
		mShadowCaster = true;
	}

	void MeshRenderer::setMesh(int meshID)
	{
		// Set the new Mesh
		mLock.lock(LockState::Write);
		mMesh = meshID;
		mLock.unlock(LockState::Write);
	}

	void MeshRenderer::setMaterial(int materialID)
	{
		// Set the new Material
		mLock.lock(LockState::Write);
		mMaterial = materialID;
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
		Mesh* mesh = RenderManager::getInstance().getMesh(mMesh);
		mLock.unlock(LockState::Read);

		return mesh;
	}

	Material* MeshRenderer::getMaterial()
	{
		// Return a raw pointer to the Material
		mLock.lock(LockState::Read);
		Material* material = RenderManager::getInstance().getMaterial(mMaterial);
		mLock.unlock(LockState::Read);

		return material;
	}

	void MeshRenderer::setShadowCaster(bool shadowCaster)
	{
		mShadowCaster = shadowCaster;
	}

	bool MeshRenderer::isShadowCaster()
	{
		return mShadowCaster;
	}

	void MeshRenderer::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Bind the mesh and material to the rendering pipeline
		RenderManager::getInstance().getMesh(mMesh)->bind(context);
		RenderManager::getInstance().getMaterial(mMaterial)->bind(context);
	}

	void MeshRenderer::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Unbind the mesh and material from the rendering pipeline
		RenderManager::getInstance().getMesh(mMesh)->unbind(context);
		RenderManager::getInstance().getMaterial(mMaterial)->unbind(context);
	}
}