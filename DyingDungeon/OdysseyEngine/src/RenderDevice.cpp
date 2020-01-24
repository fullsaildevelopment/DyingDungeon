#include "RenderDevice.h"
#include "Application.h"
#include "Buffer.h"
#include "Scene.h"
#include "Entity.h"
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
#include "Sprite2DPass.h"
#include "ScriptDataLoader.h"
#include "SceneDX11.h"

namespace Odyssey
{
	RenderDevice::RenderDevice()
	{
		createDevice();
	}

	Microsoft::WRL::ComPtr<ID3D11Device> RenderDevice::getDevice()
	{
		return mDevice.Get();
	}

	Microsoft::WRL::ComPtr<ID2D1DeviceContext> RenderDevice::getDevice2DContext()
	{
		return mDevice2DContext;
	}

	Microsoft::WRL::ComPtr<ID2D1Factory1> RenderDevice::get2DFactory()
	{
		return mFactory;
	}

	RenderDevice::~RenderDevice()
	{
		int debug = 0;
	}

	std::shared_ptr<Scene> RenderDevice::createScene()
	{
		static bool init = false;
		if (init == false)
		{
			FileManager::getInstance().initialize(shared_from_this());
			init = true;
		}
		std::shared_ptr<Scene> scene = std::make_shared<SceneDX11>(shared_from_this());
		return scene;
	}

	// TODO: DO I NEED TO DESTROY THE SCRIPT LOADER BEFORE I DESTROY THE RENDER DEVICE?

	void RenderDevice::importScene(std::shared_ptr<Scene> scene, const char* filename)
	{
		//if (mScriptLoader == nullptr)
		//	mScriptLoader = std::make_shared<ScriptDataLoader>(shared_from_this());
		//mScriptLoader->importScene(scene, filename);
	}
	
	void RenderDevice::importModel(std::shared_ptr<Entity> entity, const char* filename, bool isMultiMesh)
	{
		//if (mScriptLoader == nullptr)
		//	mScriptLoader = std::make_shared<ScriptDataLoader>(shared_from_this());
		//mScriptLoader->importModel(entity, filename, isMultiMesh);
	}

	std::shared_ptr<Buffer> RenderDevice::createBuffer(BufferBindFlag bindFlag, size_t size, UINT stride, const void* data)
	{
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(shared_from_this(), bindFlag, size, stride, data);
		return buffer;
	}

	std::shared_ptr<RenderState> RenderDevice::createRenderState(Topology topology, CullMode cullMode, FillMode fillMode, bool frontCCW, bool depthClipping, bool isShadowMap)
	{
		std::shared_ptr<RenderState> renderState = std::make_shared<RenderState>(shared_from_this(), topology, cullMode, fillMode, frontCCW, depthClipping, isShadowMap);
		return renderState;
	}

	std::shared_ptr<Material> RenderDevice::createMaterial()
	{
		std::shared_ptr<Material> material = std::make_shared<Material>(shared_from_this());
		return material;
	}

	std::shared_ptr<Material> RenderDevice::createMaterial(TextureType textureType, std::shared_ptr<Texture> texture)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>(shared_from_this(), textureType, texture);
		return material;
	}

	std::shared_ptr<SamplerState> RenderDevice::createSamplerState(ComparisonFunc comparisonFunc, D3D11_FILTER filter, int bindSlot)
	{
		std::shared_ptr<SamplerState> sampler = std::make_shared<SamplerState>(shared_from_this(), comparisonFunc, filter, bindSlot);
		return sampler;
	}

	std::shared_ptr<Shader> RenderDevice::createShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(shared_from_this(), shaderType, filename, layout, numberOfElements);
		return shader;
	}

	std::shared_ptr<Texture> RenderDevice::createRenderTexture()
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(shared_from_this());
		return texture;
	}

	std::shared_ptr<Texture> RenderDevice::createTexture(TextureType textureType, const char* filename)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(shared_from_this(), textureType, filename);
		return texture;
	}

	std::shared_ptr<Viewport> RenderDevice::createViewport(RenderWindow* renderWindow)
	{
		std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(shared_from_this(), renderWindow);
		return viewport;
	}

	std::shared_ptr<Viewport> RenderDevice::createViewport(int width, int height, int topLeftX, int topLeftY, float minDepth, float maxDepth)
	{
		std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(shared_from_this(), width, height, topLeftX, topLeftY, minDepth, maxDepth);
		return viewport;
	}

	std::shared_ptr<RenderTarget> RenderDevice::createRenderTarget(int width, int height, bool depthEnabled)
	{
		std::shared_ptr<RenderTarget> renderTarget = std::make_shared<RenderTarget>(shared_from_this(), width, height, depthEnabled);
		return renderTarget;
	}

	std::shared_ptr<RenderTarget> RenderDevice::createRenderTarget(int width, int height, bool depthEnabled, RenderWindow* renderWindow)
	{
		std::shared_ptr<RenderTarget> renderTarget = std::make_shared<RenderTarget>(shared_from_this(), width, height, depthEnabled, renderWindow);
		return renderTarget;
	}

	std::shared_ptr<Mesh> RenderDevice::createMesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(shared_from_this(), vertexList, indexList);
		return mesh;
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
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(shared_from_this(), vertexList, indexList);

		return mesh;
	}

	std::shared_ptr<ClearRenderTargetPass> RenderDevice::createClearRTVPass(std::shared_ptr<RenderWindow> renderWindow, bool clearDepth)
	{
		std::shared_ptr<ClearRenderTargetPass> renderPass = std::make_shared<ClearRenderTargetPass>(shared_from_this(), mDeviceContext, renderWindow, clearDepth);
		return renderPass;
	}

	std::shared_ptr<SkyboxPass> RenderDevice::createSkyboxPass(std::shared_ptr<RenderWindow> renderWindow)
	{
		std::shared_ptr<SkyboxPass> renderPass = std::make_shared<SkyboxPass>(shared_from_this(), mDeviceContext, renderWindow);
		return renderPass;
	}

	std::shared_ptr<ShadowPass> RenderDevice::createShadowPass(int texWidth, int texHeight)
	{
		std::shared_ptr<ShadowPass> renderPass = std::make_shared<ShadowPass>(shared_from_this(), mDeviceContext, texWidth, texHeight);
		return renderPass;
	}

	std::shared_ptr<OpaquePass> RenderDevice::createOpaquePass(std::shared_ptr<RenderWindow> renderWindow)
	{
		std::shared_ptr<OpaquePass> renderPass = std::make_shared<OpaquePass>(shared_from_this(), mDeviceContext, renderWindow);
		return renderPass;
	}

	std::shared_ptr<TransparentPass> RenderDevice::createTransparentPass(std::shared_ptr<RenderWindow> renderWindow)
	{
		std::shared_ptr<TransparentPass> renderPass = std::make_shared<TransparentPass>(shared_from_this(), mDeviceContext, renderWindow);
		return renderPass;
	}

	std::shared_ptr<DebugPass> RenderDevice::createDebugPass(std::shared_ptr<RenderWindow> renderWindow)
	{
		std::shared_ptr<DebugPass> renderPass = std::make_shared<DebugPass>(shared_from_this(), mDeviceContext, renderWindow);
		return renderPass;
	}

	std::shared_ptr<Sprite2DPass> RenderDevice::createSprite2DPass(std::shared_ptr<RenderWindow> renderWindow)
	{
		std::shared_ptr<Sprite2DPass> renderPass = std::make_shared<Sprite2DPass>(shared_from_this(), renderWindow);
		return renderPass;
	}

	void RenderDevice::createDevice()
	{
#ifdef _DEBUG
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;
#else
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif // _DEBUG
		
		// Find the graphics card adapter for device creation
		findVideoCard();

		// Create the 3D device using the highest possible feature level
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		HRESULT hr = D3D11CreateDevice(mVideoAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN,
			nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION, &mDevice, &featureLevel, &mDeviceContext);

		// Create the 2D factory
		auto options = D2D1_FACTORY_OPTIONS();
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, mFactory.GetAddressOf());

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		hr = mDevice.As(&dxgiDevice);

		// Obtain the Direct2D device for 2-D rendering.
		hr = mFactory->CreateDevice(dxgiDevice.Get(), &mDevice2D);

		hr = mDevice2D->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &mDevice2DContext);
	}

	void RenderDevice::findVideoCard()
	{
		// Create the DirectX graphics interface factory
		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)dxgiFactory.GetAddressOf());
		assert(!FAILED(hr));

		// Create an adapter list of gpu cards belonging to this machine
		std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapterList;
		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		UINT adapterIndex = 0;
		while (dxgiFactory->EnumAdapters(adapterIndex, dxgiAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			adapterList.push_back(dxgiAdapter);
			adapterIndex++;
		}

		// Reassign the adapter list as the list size
		adapterIndex = static_cast<UINT>(adapterList.size());

		// Create an adapter output list for the monitors available to this machine
		std::vector<Microsoft::WRL::ComPtr<IDXGIOutput>> adapterOutputList;
		Microsoft::WRL::ComPtr<IDXGIOutput> dxgiAdapterOutput;
		UINT adapterOutputIndex = 0;

		// Iterate through all adapters
		for (UINT i = 0; i < adapterIndex; i++)
		{
			// Iterate through all outputs until none are found
			while (adapterList[i]->EnumOutputs(adapterOutputIndex, dxgiAdapterOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
			{
				// Store the output and increment
				adapterOutputList.push_back(dxgiAdapterOutput);
				adapterOutputIndex++;
			}

			// Reset the counter to 0
			adapterOutputIndex = 0;
		}

		// Get the number of modes that will match dxgi_format_r8g8b8a8_unorm
		adapterOutputIndex = static_cast<UINT>(adapterOutputList.size());

		// Iterate through the adapter list
		DXGI_ADAPTER_DESC adapterDescription;
		std::vector<DXGI_ADAPTER_DESC> adapterDescriptionList;
		for (UINT i = 0; i < static_cast<UINT>(adapterList.size()); i++)
		{
			hr = adapterList[i]->GetDesc(&adapterDescription);
			adapterDescriptionList.push_back(adapterDescription);
		}

		UINT adapterToUse = 0;
		int videoCardMemory = static_cast<int>(adapterDescriptionList[0].DedicatedVideoMemory / 1024 / 1024);

		for (UINT i = 0; i < adapterDescriptionList.size(); i++)
		{
			int cardMemory = static_cast<int>(adapterDescriptionList[i].DedicatedVideoMemory / 1024 / 1024);
			if (videoCardMemory < cardMemory)
			{
				adapterToUse = i;
				videoCardMemory = cardMemory;
			}
		}

		mVideoAdapter = adapterList[adapterToUse];
	}
}