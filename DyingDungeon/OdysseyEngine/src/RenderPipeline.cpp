#include "RenderPipeline.h"
#include "RenderManager.h"
#include "SceneDX11.h"
#include "Entity.h"
#include "UICanvas.h"
#include "EventManager.h"
#include "Camera.h"
#include "Entity.h"
#include "Transform.h"
#include "RenderWindowDX11.h"
#include "RenderTarget.h"
#include "SamplerState.h"

namespace Odyssey
{
	RenderPipeline::RenderPipeline()
	{
		// Create the per frame buffer and assign it in the render arguments
		int perFrame = RenderManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(PerFrameBuffer), nullptr);
		args.perFrameBuffer = RenderManager::getInstance().getBuffer(perFrame);

		// Create the per object buffer and assign it in the render arguments
		int perObject = RenderManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(PerObjectBuffer), nullptr);
		args.perObjectBuffer = RenderManager::getInstance().getBuffer(perObject);

		linearSampler = RenderManager::getInstance().createSamplerState(ComparisonFunc::COMPARISON_NEVER, D3D11_FILTER_ANISOTROPIC, 0);
		shadowSampler = RenderManager::getInstance().createSamplerState(ComparisonFunc::COMPARISON_LESS, D3D11_FILTER_COMPARISON_ANISOTROPIC, 1);
	}

	void RenderPipeline::addRenderPass(std::shared_ptr<RenderPass> renderPass)
	{
		mRenderPasses.emplace_back(renderPass);
	}

	void RenderPipeline::render(RenderWindowDX11* activeWindow, SceneDX11* scene)
	{
		// Generate the render arguments from the scene
		generateRenderArgs(activeWindow, scene);

		// Get the render package from the scene
		mRenderPackage = scene->getRenderPackage();

		// Get the context
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = RenderManager::getInstance().getDX11Context();

		// Clear the render target associated with the active window
		activeWindow->get3DRenderTarget()->clearRenderView(context);
		activeWindow->get3DRenderTarget()->clearDepth(context);

		// Bind the two default sampler states
		RenderManager::getInstance().getSamplerState(linearSampler)->bind(context);
		RenderManager::getInstance().getSamplerState(shadowSampler)->bind(context);

		// Iterate over each render pass in the list
		for (std::shared_ptr<RenderPass> pass : mRenderPasses)
		{
			// Check if the pass is enabled
			if (pass->isEnabled())
			{
				// Perform pre-render and render processs
				pass->preRender(args, mRenderPackage);
				pass->render(args, mRenderPackage);
			}
		}
	}

	RenderPass* RenderPipeline::getRenderPass(int index)
	{
		return mRenderPasses[index].get();
	}

	void RenderPipeline::generateRenderArgs(RenderWindowDX11* activeWindow, SceneDX11* scene)
	{
		// Ensure there is a camera in the scene
		if (Camera* camera = scene->getMainCamera()->getComponent<Camera>())
		{
			// Get the inverse view matrix
			args.perFrame.view = camera->getInverseViewMatrix();

			// Calculate and set view proj
			DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&args.perFrame.view), DirectX::XMLoadFloat4x4(&camera->getProjectionMatrix()));
			DirectX::XMStoreFloat4x4(&args.perFrame.viewProj, viewProj);
		}

		// Store the dx11 and d2d context
		args.context = RenderManager::getInstance().getDX11Context();
		args.context2D = RenderManager::getInstance().getD2DContext();

		// Store the active window
		args.activeWindow = activeWindow;
	}
}