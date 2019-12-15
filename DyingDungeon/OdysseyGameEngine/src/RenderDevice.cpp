#include "RenderDevice.h"
#include "Application.h"
#include "Buffer.h"
#include "Scene.h"
#include "ClearRenderTargetPass.h"
#include "SkyboxPass.h"
#include "ShadowPass.h"
#include "OpaquePass.h"
#include "TransparentPass.h"
#include "DebugPass.h"
#include "RenderState.h"
#include "Light.h"
#include "Material.h"
#include "SamplerState.h"
#include "Shader.h"
#include "Texture.h"
#include "Viewport.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "FileManager.h"
#include "Mesh.h"

namespace Odyssey
{
	RenderDevice::RenderDevice(Application& application)
	{
		createDevice(application.GetModuleHandle());
		FileManager::getInstance().initialize(this);
	}

	Microsoft::WRL::ComPtr<ID3D11Device> RenderDevice::getDevice()
	{
		return mDevice.Get();
	}

	std::shared_ptr<Scene> RenderDevice::createScene()
	{
		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		return scene;
	}

	std::shared_ptr<Buffer> RenderDevice::createBuffer(BufferBindFlag bindFlag, size_t size, UINT stride, const void* data)
	{
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(*this, bindFlag, size, stride, data);
		return buffer;
	}

	std::shared_ptr<RenderState> RenderDevice::createRenderState(Topology topology, CullMode cullMode, FillMode fillMode, bool frontCCW, bool depthClipping, bool isShadowMap)
	{
		std::shared_ptr<RenderState> renderState = std::make_shared<RenderState>(*this, topology, cullMode, fillMode, frontCCW, depthClipping, isShadowMap);
		return renderState;
	}

	std::shared_ptr<Material> RenderDevice::createMaterial()
	{
		std::shared_ptr<Material> material = std::make_shared<Material>(*this);
		return material;
	}

	std::shared_ptr<Material> RenderDevice::createMaterial(TextureType textureType, std::shared_ptr<Texture> texture)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>(*this, textureType, texture);
		return material;
	}

	std::shared_ptr<SamplerState> RenderDevice::createSamplerState(ComparisonFunc comparisonFunc, D3D11_FILTER filter, int bindSlot)
	{
		std::shared_ptr<SamplerState> sampler = std::make_shared<SamplerState>(*this, comparisonFunc, filter, bindSlot);
		return sampler;
	}

	std::shared_ptr<Shader> RenderDevice::createShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(*this, shaderType, filename, layout, numberOfElements);
		return shader;
	}

	std::shared_ptr<Texture> RenderDevice::createRenderTexture()
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(*this);
		return texture;
	}

	std::shared_ptr<Texture> RenderDevice::createTexture(TextureType textureType, const char* filename)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(*this, textureType, filename);
		return texture;
	}

	std::shared_ptr<Viewport> RenderDevice::createViewport(RenderWindow* renderWindow)
	{
		std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(*this, renderWindow);
		return viewport;
	}

	std::shared_ptr<Viewport> RenderDevice::createViewport(int width, int height, int topLeftX, int topLeftY, float minDepth, float maxDepth)
	{
		std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(*this, width, height, topLeftX, topLeftY, minDepth, maxDepth);
		return viewport;
	}

	std::shared_ptr<RenderTarget> RenderDevice::createRenderTarget(int width, int height, bool depthEnabled)
	{
		std::shared_ptr<RenderTarget> renderTarget = std::make_shared<RenderTarget>(*this, width, height, depthEnabled);
		return renderTarget;
	}

	std::shared_ptr<RenderTarget> RenderDevice::createRenderTarget(int width, int height, bool depthEnabled, RenderWindow* renderWindow)
	{
		std::shared_ptr<RenderTarget> renderTarget = std::make_shared<RenderTarget>(*this, width, height, depthEnabled, renderWindow);
		return renderTarget;
	}

	std::shared_ptr<Mesh> RenderDevice::createCube(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 center)
	{
		std::vector<Vertex> vertexList;
		vertexList.resize(8);
		vertexList[0].position = DirectX::XMFLOAT3(-scale.x + center.x, -scale.y + center.y, -scale.z + center.z);
		vertexList[1].position = DirectX::XMFLOAT3(-scale.x + center.x, +scale.y + center.y, -scale.z + center.z);
		vertexList[2].position = DirectX::XMFLOAT3(+scale.x + center.x, +scale.y + center.y, -scale.z + center.z);
		vertexList[3].position = DirectX::XMFLOAT3(+scale.x + center.x, -scale.y + center.y, -scale.z + center.z);
		vertexList[4].position = DirectX::XMFLOAT3(-scale.x + center.x, -scale.y + center.y, +scale.z + center.z);
		vertexList[5].position = DirectX::XMFLOAT3(-scale.x + center.x, +scale.y + center.y, +scale.z + center.z);
		vertexList[6].position = DirectX::XMFLOAT3(+scale.x + center.x, +scale.y + center.y, +scale.z + center.z);
		vertexList[7].position = DirectX::XMFLOAT3(+scale.x + center.x, -scale.y + center.y, +scale.z + center.z);

		std::vector<unsigned int> indexList
		{
			// front face
			0, 1, 2, 0, 2, 3,
			// back face
			4, 6, 5, 4, 7, 6,
			// left face
			4, 5, 1, 4, 1, 0,
			// right face
			3, 2, 6, 3, 6, 7,
			// top face
			1, 5, 6, 1, 6, 2,
			// bottom face
			4, 0, 3, 4, 3, 7
		};

		// Create the mesh and set it's index/vertex data
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(*this, vertexList, indexList);

		return mesh;
	}

	std::shared_ptr<ClearRenderTargetPass> RenderDevice::createClearRTVPass(std::shared_ptr<RenderTarget> renderTarget, bool clearDepth)
	{
		std::shared_ptr<ClearRenderTargetPass> renderPass = std::make_shared<ClearRenderTargetPass>(*this, renderTarget, clearDepth);
		return renderPass;
	}

	std::shared_ptr<SkyboxPass> RenderDevice::createSkyboxPass(const char* textureFilename, std::shared_ptr<RenderTarget> renderTarget)
	{
		std::shared_ptr<SkyboxPass> renderPass = std::make_shared<SkyboxPass>(*this, textureFilename, renderTarget);
		return renderPass;
	}

	std::shared_ptr<ShadowPass> RenderDevice::createShadowPass(std::shared_ptr<Light> shadowLight, int texWidth, int texHeight)
	{
		std::shared_ptr<ShadowPass> renderPass = std::make_shared<ShadowPass>(*this, shadowLight, texWidth, texHeight);
		return renderPass;
	}

	std::shared_ptr<OpaquePass> RenderDevice::createOpaquePass(std::shared_ptr<RenderTarget> renderTarget)
	{
		std::shared_ptr<OpaquePass> renderPass = std::make_shared<OpaquePass>(*this, renderTarget);
		return renderPass;
	}

	std::shared_ptr<TransparentPass> RenderDevice::createTransparentPass(std::shared_ptr<RenderTarget> renderTarget)
	{
		std::shared_ptr<TransparentPass> renderPass = std::make_shared<TransparentPass>(*this, renderTarget);
		return renderPass;
	}

	std::shared_ptr<DebugPass> RenderDevice::createDebugPass(std::shared_ptr<RenderTarget> renderTarget)
	{
		std::shared_ptr<DebugPass> renderPass = std::make_shared<DebugPass>(*this, renderTarget);
		return renderPass;
	}

	void RenderDevice::createDevice(HINSTANCE hInstance)
	{
		const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};

		UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;

		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION, &mDevice, &featureLevel, &mDeviceContext);
	}
}