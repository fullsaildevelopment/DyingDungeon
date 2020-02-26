#include "SkyboxPass.h"
#include "RenderState.h"
#include "SamplerState.h"
#include "Shader.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "RenderManager.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	SkyboxPass::SkyboxPass()
	{
		mRenderState = RenderManager::getInstance().createRenderState(Topology::TriangleList, CullMode::CULL_NONE, FillMode::FILL_SOLID, true, true, false);

		D3D11_INPUT_ELEMENT_DESC vShaderLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		mVertexShader = RenderManager::getInstance().createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/SkyboxVertexShader.cso", vShaderLayout, 7);
		mPixelShader = RenderManager::getInstance().createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/SkyboxPixelShader.cso", nullptr);
	}

	void SkyboxPass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Update the per frame buffer
		updatePerFrameBuffer(args.context, args.perFrame, args.perFrameBuffer);

		// Bind the active window as the render target
		args.activeWindow->get3DRenderTarget()->bind(args.context);

		// Bind the render state and vertex/pixel shaders
		RenderManager& renderManager = RenderManager::getInstance();
		renderManager.getRenderState(mRenderState)->bind(args.context);
		renderManager.getShader(mVertexShader)->bind(args.context);
		renderManager.getShader(mPixelShader)->bind(args.context);
	}

	void SkyboxPass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Set the skybox to the camera's position
		renderPackage.skybox.transform->setPosition(renderPackage.cameraPosition.x, renderPackage.cameraPosition.y, renderPackage.cameraPosition.z);

		// Get the object's global transform and set the MVP acoordingly
		args.perObject.world = renderPackage.skybox.transform->getLocalTransform();

		// Update and bind the constant buffer
		updatePerObjectBuffer(args.context, args.perObject, args.perObjectBuffer);

		// Draw the skybox
		if (renderPackage.skybox.meshRenderer)
		{
			renderPackage.skybox.meshRenderer->bind(args.context);
			args.context->DrawIndexed(renderPackage.skybox.meshRenderer->getMesh()->getNumberOfIndices(), 0, 0);
		}

		// Clear the depth of the render targets
		args.activeWindow->get3DRenderTarget()->clearDepth(args.context);
	}
}