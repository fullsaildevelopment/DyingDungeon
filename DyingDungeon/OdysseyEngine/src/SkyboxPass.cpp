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
#include "RenderDevice.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	SkyboxPass::SkyboxPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, const char* skyboxTexture, std::shared_ptr<RenderWindow> renderWindow)
	{
		mDeviceContext = context;
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

		std::shared_ptr<Texture> texture = renderDevice->createTexture(TextureType::Skybox, skyboxTexture);
		std::shared_ptr<Mesh> mesh = renderDevice->createCube(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		std::shared_ptr<Material> material = renderDevice->createMaterial(TextureType::Skybox, texture);

		mSkyBox = std::make_shared<Entity>();
		mSkyBox->addComponent<MeshRenderer>(mesh, material);
		mSkyBox->addComponent<Transform>();

		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(renderWindow);
		mRenderState = renderDevice->createRenderState(Topology::TriangleList, CullMode::CULL_NONE, FillMode::FILL_SOLID, true, true, false);

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
		mVertexShader = renderDevice->createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/SkyboxVertexShader.cso", vShaderLayout, 7);
		mPixelShader = renderDevice->createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/SkyboxPixelShader.cso", nullptr);
	}

	void SkyboxPass::preRender(RenderArgs& args)
	{
		// Update the buffer
		updatePerFrameBuffer(mDeviceContext, args.perFrame, args.perFrameBuffer);
		mRenderWindow->get3DRenderTarget()->bind(mDeviceContext);
		mRenderState->bind(mDeviceContext);
		mVertexShader->bind(mDeviceContext);
		mPixelShader->bind(mDeviceContext);
	}

	void SkyboxPass::render(RenderArgs& args)
	{
		// Get the camera's position
		DirectX::XMFLOAT3 camPos;
		camPos = args.camera->getComponent<Transform>()->getPosition();

		// Set the skybox to the camera's position
		mSkyBox->getComponent<Transform>()->setPosition(camPos.x, camPos.y, camPos.z);

		// Get the object's global transform and set the MVP acoordingly
		args.perObject.world = mSkyBox->getComponent<Transform>()->getLocalTransform();

		// Update and bind the constant buffer
		updatePerObjectBuffer(mDeviceContext, args.perObject, args.perObjectBuffer);

		// Draw the skybox
		if (mSkyBox->getComponent<MeshRenderer>())
		{
			mSkyBox->getComponent<MeshRenderer>()->bind(mDeviceContext);
			mDeviceContext->DrawIndexed(mSkyBox->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);
		}

		// Clear the depth of the render targets
		mRenderWindow->get3DRenderTarget()->clearDepth(mDeviceContext);
	}
}