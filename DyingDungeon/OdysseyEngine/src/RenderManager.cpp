#include "BlendState.h"
#include "Buffer.h"
#include "ClearRenderTargetPass.h"
#include "DebugPass.h"
#include "DepthState.h"
#include "Entity.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "OpaquePass.h"
#include "RenderState.h"
#include "RenderTarget.h"
#include "RenderWindow.h"
#include "SamplerState.h"
#include "Scene.h"
#include "ScriptDataLoader.h"
#include "Shader.h"
#include "ShadowPass.h"
#include "SkyboxPass.h"
#include "Sprite2DPass.h"
#include "Texture.h"
#include "TransparentPass.h"
#include "Viewport.h"
#include "RenderManager.h"

namespace Odyssey
{
	RenderManager::RenderManager()
	{
		createDX11Device();
		createD2DDevice();
		mScriptLoader = std::make_shared<ScriptDataLoader>();
		mBufferIDs.push(0);
		mRenderTargetIDs.push(0);
	}

	RenderManager& RenderManager::getInstance()
	{
		// TODO: insert return statement here
		static RenderManager instance;
		return instance;
	}

	Microsoft::WRL::ComPtr<ID3D11Device> RenderManager::getDX11Device()
	{
		return mDevice;
	}

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> RenderManager::getDX11Context()
	{
		return mDeviceContext;
	}

	Microsoft::WRL::ComPtr<ID2D1DeviceContext> RenderManager::getD2DContext()
	{
		return mDevice2DContext;
	}

	Microsoft::WRL::ComPtr<ID2D1Factory1> RenderManager::getD2DFactory()
	{
		return mFactory2D;
	}

	void RenderManager::importModel(Entity*& entity, const char* filename, bool isMultiMesh)
	{
		mScriptLoader->importModel(entity, filename, isMultiMesh);
	}

	void RenderManager::importScene(Scene* scene, const char* filename)
	{
		mScriptLoader->importScene(scene, filename);
	}

	int RenderManager::createBuffer(BufferBindFlag bindFlag, size_t size, UINT stride, const void* data)
	{
		// Create the buffer
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(bindFlag, size, stride, data);
		
		// Get the next render target ID and pop it off the top
		int ID = mBufferIDs.top();
		mBufferIDs.pop();

		// Check if this is an already allocated slot
		if (ID < mBuffers.size())
		{
			// Assign the render target
			mBuffers[ID] = buffer;
		}
		else
		{
			// Allocate a slot for the render target and push the next slot onto the stack
			mBuffers.push_back(buffer);
			mBufferIDs.push(mBuffers.size());
		}

		// Return the ID
		return ID;
	}
	
	int RenderManager::createMaterial()
	{
		// Create the material
		std::shared_ptr<Material> material = std::make_shared<Material>();
		// Store the ID of the material
		int ID = mMaterials.size();
		// Add the material to the list
		mMaterials.push_back(material);
		// Return the ID
		return ID;
	}

	int RenderManager::createMaterial(TextureType textureType, int textureID)
	{
		// Create the material
		std::shared_ptr<Material> material = std::make_shared<Material>(textureType, textureID);
		// Store the ID of the material
		int ID = mMaterials.size();
		// Add the material to the list
		mMaterials.push_back(material);
		// Return the ID
		return ID;
	}

	int RenderManager::createRenderState(Topology topology, CullMode cullMode, FillMode fillMode, bool frontCCW, bool depthClipping, bool isShadowMap)
	{
		// Create the render state
		std::shared_ptr<RenderState> renderState = std::make_shared<RenderState>(topology, cullMode, fillMode, frontCCW, depthClipping, isShadowMap);
		// Store the ID of the render state
		int ID = mRenderStates.size();
		// Add the render state to the list
		mRenderStates.push_back(renderState);
		// Return the ID
		return ID;
	}

	int RenderManager::createSamplerState(ComparisonFunc comparisonFunc, D3D11_FILTER filter, int bindSlot)
	{
		// Create the sampler state
		std::shared_ptr<SamplerState> samplerState = std::make_shared<SamplerState>(comparisonFunc, filter, bindSlot);
		// Store the ID of the sampler state
		int ID = mSamplerStates.size();
		// Add the sampler state to the list
		mSamplerStates.push_back(samplerState);
		// Return the ID
		return ID;
	}

	int RenderManager::createDepthState(bool depthWriteEnable, bool depthReadEnable, ComparisonFunc comparisonFunc)
	{
		// Create the depth state
		std::shared_ptr<DepthState> depthState = std::make_shared<DepthState>(depthWriteEnable, depthReadEnable, comparisonFunc);
		// Store the ID of the depth state
		int ID = mDepthStates.size();
		// Add the depth state to the list
		mDepthStates.push_back(depthState);
		// Return the ID
		return ID;
	}

	int RenderManager::createBlendState(BlendDesc& blendDesc, float blendFactor[4])
	{
		// Create the blend state
		std::shared_ptr<BlendState> blendState = std::make_shared<BlendState>(blendDesc, blendFactor);
		// Store the ID of the blend state
		int ID = mBlendStates.size();
		// Add the blend state to the list
		mBlendStates.push_back(blendState);
		// Return the ID
		return ID;
	}

	int RenderManager::createShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements)
	{
		// Create the blend state
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderType, filename, layout, numberOfElements);
		// Store the ID of the blend state
		int ID = mShaders.size();
		// Add the blend state to the list
		mShaders.push_back(shader);
		// Return the ID
		return ID;
	}

	int RenderManager::createTexture()
	{
		// Create the texture
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		// Store the ID of the texture
		int ID = mTextures.size();
		// Add the texture to the list
		mTextures.push_back(texture);
		// Return the ID
		return ID;
	}

	int RenderManager::createTexture(TextureType textureType, const char* filename)
	{
		// Create the texture
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(textureType, filename);
		// Store the ID of the texture
		int ID = mTextures.size();
		// Add the texture to the list
		mTextures.push_back(texture);
		// Return the ID
		return ID;
	}

	int RenderManager::createTexture(TextureType textureType, int width, int height, DXGI_FORMAT format, UINT bindFlags)
	{
		// Create the texture
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(textureType, width, height, format, bindFlags);
		// Store the ID of the texture
		int ID = mTextures.size();
		// Add the texture to the list
		mTextures.push_back(texture);
		// Return the ID
		return ID;
	}

	int RenderManager::createViewport(RenderWindow* renderWindow)
	{
		// Create the texture
		std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(renderWindow);
		// Store the ID of the texture
		int ID = mViewports.size();
		// Add the texture to the list
		mViewports.push_back(viewport);
		// Return the ID
		return ID;
	}

	int RenderManager::createViewport(int width, int height, int topLeftX, int topLeftY, float minDepth, float maxDepth)
	{
		// Create the viewport
		std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(width, height, topLeftX, topLeftY, minDepth, maxDepth);
		// Store the ID of the viewport
		int ID = mViewports.size();
		// Add the viewport to the list
		mViewports.push_back(viewport);
		// Return the ID
		return ID;
	}

	int RenderManager::createRenderTarget(int width, int height, bool depthEnabled)
	{
		// Create the render target
		std::shared_ptr<RenderTarget> renderTarget = std::make_shared<RenderTarget>(width, height, depthEnabled);

		// Get the next render target ID and pop it off the top
		int ID = mRenderTargetIDs.top();
		mRenderTargetIDs.pop();

		// Check if this is an already allocated slot
		if (ID < mRenderTargets.size())
		{
			// Assign the render target
			mRenderTargets[ID] = renderTarget;
		}
		else
		{
			// Allocate a slot for the render target and push the next slot onto the stack
			mRenderTargets.push_back(renderTarget);
			mRenderTargetIDs.push(mRenderTargets.size());
		}

		// Return the ID
		return ID;
	}

	int RenderManager::createRenderTarget(RenderWindow* renderWindow, bool depthEnabled)
	{
		// Create the render target
		std::shared_ptr<RenderTarget> renderTarget = std::make_shared<RenderTarget>(renderWindow, depthEnabled);

		// Get the next render target ID and pop it off the top
		int ID = mRenderTargetIDs.top();
		mRenderTargetIDs.pop();

		// Check if this is an already allocated slot
		if (ID < mRenderTargets.size())
		{
			// Assign the render target
			mRenderTargets[ID] = renderTarget;
		}
		else
		{
			// Allocate a slot for the render target and push the next slot onto the stack
			mRenderTargets.push_back(renderTarget);
			mRenderTargetIDs.push(mRenderTargets.size());
		}

		// Return the ID
		return ID;
	}

	int RenderManager::createMesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList)
	{
		// Create the render target
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertexList, indexList);
		// Store the ID of the render target
		int ID = mMeshes.size();
		// Add the render target to the list
		mMeshes.push_back(mesh);
		// Return the ID
		return ID;
	}

	int RenderManager::createCube(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 center)
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

		return createMesh(vertexList, indexList);
	}

	Buffer* RenderManager::getBuffer(int ID)
	{
		// Check for out of bounds
		if (ID >= mBuffers.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mBuffers[ID].get();
	}

	Mesh* RenderManager::getMesh(int ID)
	{
		// Check for out of bounds
		if (ID >= mMeshes.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mMeshes[ID].get();
	}

	Material* RenderManager::getMaterial(int ID)
	{
		// Check for out of bounds
		if (ID >= mMaterials.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mMaterials[ID].get();
	}

	Texture* RenderManager::getTexture(int ID)
	{
		// Check for out of bounds
		if (ID >= mTextures.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mTextures[ID].get();
	}

	Viewport* RenderManager::getViewport(int ID)
	{
		// Check for out of bounds
		if (ID >= mTextures.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mViewports[ID].get();
	}

	RenderState* RenderManager::getRenderState(int ID)
	{
		// Check for out of bounds
		if (ID >= mRenderStates.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mRenderStates[ID].get();
	}

	SamplerState* RenderManager::getSamplerState(int ID)
	{
		// Check for out of bounds
		if (ID >= mSamplerStates.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mSamplerStates[ID].get();
	}

	DepthState* RenderManager::getDepthState(int ID)
	{
		// Check for out of bounds
		if (ID >= mDepthStates.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mDepthStates[ID].get();
	}

	BlendState* RenderManager::getBlendState(int ID)
	{
		// Check for out of bounds
		if (ID >= mBlendStates.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mBlendStates[ID].get();
	}

	Shader* RenderManager::getShader(int ID)
	{
		// Check for out of bounds
		if (ID >= mShaders.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mShaders[ID].get();
	}

	RenderTarget* RenderManager::getRenderTarget(int ID)
	{
		// Check for out of bounds
		if (ID >= mRenderTargets.size())
		{
			return nullptr;
		}

		// Return the raw pointer stored at the ID index
		return mRenderTargets[ID].get();
	}

	void RenderManager::destroyBuffer(int ID)
	{
		if (ID < mBuffers.size())
		{
			mBufferIDs.push(ID);
			mBuffers[ID] = nullptr;
		}
	}

	void RenderManager::destroyRenderTarget(int ID)
	{
		if (ID < mRenderTargets.size())
		{
			mRenderTargetIDs.push(ID);
			mRenderTargets[ID] = nullptr;
		}
	}

	void RenderManager::createDX11Device()
	{
#ifdef _DEBUG
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;
#else
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif // _DEBUG

		// Find the graphics card adapter for device creation
		findVideoAdapter();

		// Create the 3D device using the highest possible feature level
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		HRESULT hr = D3D11CreateDevice(mVideoAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN,
			nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION, &mDevice, &featureLevel, &mDeviceContext);
	}

	void RenderManager::createD2DDevice()
	{
		// Create the 2D factory
		auto options = D2D1_FACTORY_OPTIONS();
		//options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, mFactory2D.GetAddressOf());

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		hr = mDevice.As(&dxgiDevice);

		// Obtain the Direct2D device for 2-D rendering.
		hr = mFactory2D->CreateDevice(dxgiDevice.Get(), &mDevice2D);

		hr = mDevice2D->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &mDevice2DContext);
	}

	void RenderManager::findVideoAdapter()
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