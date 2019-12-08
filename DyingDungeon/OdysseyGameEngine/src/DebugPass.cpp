#include "DebugPass.h"
#include "DebugManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "GameObject.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Light.h"

namespace Odyssey
{
	DebugPass::DebugPass(std::shared_ptr<RenderTarget> renderTarget)
	{
		mDevice = RenderManager::getInstance().getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		mRenderTarget = renderTarget;
		mRenderState = std::make_shared<RenderState>(Topology::LineList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, true, false);

		// Create the input layout
		D3D11_INPUT_ELEMENT_DESC cvLayout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		mVertexShader = ShaderManager::getInstance().createShader(ShaderType::VertexShader, "../OdysseyGameEngine/shaders/DebugVertexShader.cso", cvLayout, 2);
		mPixelShader = ShaderManager::getInstance().createShader(ShaderType::PixelShader, "../OdysseyGameEngine/shaders/DebugPixelShader.cso", nullptr);

		DebugManager::getInstance().initialize();
	}

	void DebugPass::preRender(RenderArgs& args)
	{
		args.shaderMatrix.view = args.camera->getInverseViewMatrix();
		args.shaderMatrix.proj = args.camera->getProjectionMatrix();
		updateShaderMatrixBuffer(args.shaderMatrix, args.shaderMatrixBuffer);

		mRenderTarget->bind();
		mVertexShader->bind();
		mPixelShader->bind();
		mRenderState->bind();
		DebugManager::getInstance().clearBuffer();
	}

	void DebugPass::render(RenderArgs& args)
	{
		for (std::shared_ptr<GameObject> debugObject : args.renderList)
		{
			if (Animator* animator = debugObject->getComponent<Animator>())
			{
				if (animator->getDebugEnabled())
				{
					animator->debugDraw({ 1.0f, 0.0f, 0.0f });
				}
			}

			if (AABB* aabb = debugObject->getComponent<AABB>())
			{
				aabb->debugDraw({ 0,0,1 });
			}

			for (std::shared_ptr<GameObject> child : debugObject->getChildren())
			{
				if (Animator* animator = child->getComponent<Animator>())
				{
					if (animator->getDebugEnabled())
					{
						animator->debugDraw({ 1.0f, 0.0f, 0.0f });
					}
				}

				if (AABB* aabb = child->getComponent<AABB>())
				{
					child->getComponent<AABB>()->debugDraw({ 0,0,1 });
					aabb->debugDraw({ 0,0,1 });
				}
			}
		}

		for (std::shared_ptr<Light> lightObject : args.lightList)
		{
			lightObject->debugDraw();
		}

		DebugManager::getInstance().bind();
		mDeviceContext->Draw(DebugManager::getInstance().getNumberOfVertices(), 0);
	}
}