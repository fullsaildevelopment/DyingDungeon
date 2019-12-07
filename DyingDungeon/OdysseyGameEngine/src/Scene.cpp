#include "Scene.h"
#include "BufferManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "SceneObject.h"
#include "RenderPass.h"
#include "RenderPipelineManager.h"
#include "ParticleSystem.h"
#include "Buffer.h"

namespace Odyssey
{
	Scene::Scene()
	{
		mLightingBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(SceneLighting), nullptr);
		mShaderMatrixBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(ShaderMatrix), nullptr);
		mXTimer.Restart();
	}

	void Scene::setSkybox(const char* textureFilename)
	{
		int texID = TextureManager::getInstance().importTexture(TextureType::Skybox, textureFilename);
		std::shared_ptr<Mesh> mesh = MeshManager::getInstance().createCube(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		std::shared_ptr<Material> material = MaterialManager::getInstance().createMaterial();

		mSkybox = std::make_shared<SceneObject>();
		mSkybox->attachMeshRenderer(mesh, material);
		mSkybox->getMeshRenderer()->getMaterial()->setTexture(TextureType::Skybox, texID);
	}

	std::shared_ptr<SceneObject> Scene::getSkybox()
	{
		return mSkybox;
	}

	void Scene::addLight(std::shared_ptr<Light> light)
	{
		mSceneLights.push_back(light);
	}

	void Scene::addSceneObject(std::shared_ptr<SceneObject> sceneObject)
	{
		mSceneObjectList.push_back(sceneObject);
	}

	void Scene::render()
	{
		mXTimer.Signal();

		generateLightList();
		// Generate a render list and perform the render pipeline process on all renderable objects
		renderScene();
		mDeltaTime = mXTimer.SmoothDelta();
	}

	double Scene::getDeltaTime()
	{
		return mDeltaTime;
	}

	void Scene::renderScene()
	{
		RenderArgs args;
		args.shaderMatrixBuffer = mShaderMatrixBuffer;
		args.camera = &mMainCamera;
		std::vector<std::shared_ptr<SceneObject>> renderList;
		args.lightList = mSceneLights;

		// Generate a render list
		for (std::shared_ptr<SceneObject> renderObject : mSceneObjectList)
		{
			// If the object has an animator, perform the update
			if (renderObject->hasAnimator())
			{
				renderObject->getAnimator()->update(mDeltaTime);
			}

			if (renderObject->hasParticleSystem())
			{
				args.transparentList.push_back(renderObject);
			}

			// If the object has a mesh renderer push it, and its children, onto the render list
			if (renderObject->hasMeshRenderer())
			{
				args.renderList.push_back(renderObject);
			}
			else
			{
				// If the parent object doesn't have a mesh renderer, search the children for a mesh renderer
				for (std::shared_ptr<SceneObject> child : renderObject->getChildren())
				{
					// The child has a mesh renderer, push it's parent, and the parent's children onto the render list
					if (child->hasMeshRenderer())
					{
						args.renderList.push_back(renderObject);
						break;
					}
				}
			}
		}

		RenderPipelineManager::getInstance().render(args);
	}

	void Scene::renderDebug(ShaderMatrix& bufferMatrices)
	{
		// Set the world matrix as identity
		DirectX::XMFLOAT4X4 identity;
		DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());

		// Iterate over the entire debug list
		for (int i = 0; i < mDebugList.size(); i++)
		{
			// Debug draw each animator in the list
			if (mDebugList[i]->hasAnimator() && mDebugList[i]->getAnimator()->getDebugEnabled())
			{
				mDebugList[i]->getAnimator()->debugDraw({ 1,0,0 });
			}
		}
		mDebugList.clear();
	}

	void Scene::generateLightList()
	{
		// Generate a list of lights on a per-object basis
		SceneLighting sceneLighting;
		sceneLighting.numLights = 0;

		// Set the camera's position for specular highlighting
		sceneLighting.camPos = DirectX::XMFLOAT3(mMainCamera.getViewMatrix().m[3][0], mMainCamera.getViewMatrix().m[3][1], mMainCamera.getViewMatrix().m[3][2]);

		// Iterate through the entire scene light list
		for (int i = 0; i < mSceneLights.size(); i++)
		{
			// Directional and spot lights are automatically added to the light list
			sceneLighting.sceneLights[sceneLighting.numLights] = *(mSceneLights[i]);
			sceneLighting.numLights++;
		}

		// Set the lighting constant buffer
		mLightingBuffer->updateData(&sceneLighting);
		mLightingBuffer->bind(1, ShaderType::PixelShader);
	}
}