#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class BlendState;
	class Buffer;
	class ClearRenderTargetPass;
	class DebugPass;
	class DepthState;
	class Entity;
	class Light;
	class Material;
	class Mesh;
	class OpaquePass;
	class RenderState;
	class RenderTarget;
	class RenderWindow;
	class SamplerState;
	class Scene;
	class ScriptDataLoader;
	class Shader;
	class ShadowPass;
	class SkyboxPass;
	class Sprite2DPass;
	class Texture;
	class TransparentPass;
	class Viewport;

	class RenderManager
	{
	public:
		static RenderManager& getInstance();
		~RenderManager() { }
	public:
		Microsoft::WRL::ComPtr<ID3D11Device> getDX11Device();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> getDX11Context();
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> getD2DContext();
		Microsoft::WRL::ComPtr<ID2D1Factory1> getD2DFactory();
	public:
		void importModel(Entity*& entity, const char* filename, bool isMultiMesh);
		void importScene(Scene* scene, const char* filename);
	public:
		int createBuffer(BufferBindFlag bindFlag, size_t size, UINT stride, const void* data);
		int createMaterial();
		int createMaterial(TextureType textureType, int textureID);
		int createRenderState(Topology topology, CullMode cullMode, FillMode fillMode, bool frontCCW = false, bool depthClipping = true, bool isShadowMap = false);
		int createSamplerState(ComparisonFunc comparisonFunc, D3D11_FILTER filter, int bindSlot);
		int createDepthState(bool depthWriteEnable, bool depthReadEnable, ComparisonFunc comparisonFunc);
		int createBlendState(BlendDesc& blendDesc, float blendFactor[4]);
		int createShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements = 0);
		int createTexture();
		int createTexture(TextureType textureType, const char* filename);
		int createTexture(TextureType textureType, int width, int height, DXGI_FORMAT format, UINT bindFlags);
		int createViewport(RenderWindow* renderWindow);
		int createViewport(int width, int height, int topLeftX, int topLeftY, float minDepth, float maxDepth);
		int createRenderTarget(int width, int height, bool depthEnabled);
		int createRenderTarget(RenderWindow* renderWindow, bool depthEnabled);
		int createMesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList);
		int createCube(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 center);
	public:
		Buffer* getBuffer(int ID);
		Mesh* getMesh(int ID);
		Material* getMaterial(int ID);
		Texture* getTexture(int ID);
		Viewport* getViewport(int ID);
		RenderState* getRenderState(int ID);
		SamplerState* getSamplerState(int ID);
		DepthState* getDepthState(int ID);
		BlendState* getBlendState(int ID);
		Shader* getShader(int ID);
		RenderTarget* getRenderTarget(int ID);
	private:
		RenderManager();
		void createDX11Device();
		void createD2DDevice();
		void findVideoAdapter();
	private: // Rendering Interfaces
		// Direct3D
		Microsoft::WRL::ComPtr<IDXGIAdapter> mVideoAdapter;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		// Direct2D
		Microsoft::WRL::ComPtr<ID2D1Factory1> mFactory2D;
		Microsoft::WRL::ComPtr<ID2D1Device> mDevice2D;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> mDevice2DContext;
		// Script loading
		std::shared_ptr<ScriptDataLoader> mScriptLoader;
	private: // Typedefs
		typedef std::vector<std::shared_ptr<Buffer>> BufferList;
		typedef std::vector<std::shared_ptr<Material>> MaterialList;
		typedef std::vector<std::shared_ptr<RenderState>> RenderStateList;
		typedef std::vector<std::shared_ptr<SamplerState>> SamplerStateList;
		typedef std::vector<std::shared_ptr<DepthState>> DepthStateList;
		typedef std::vector<std::shared_ptr<BlendState>> BlendStateList;
		typedef std::vector<std::shared_ptr<Texture>> TextureList;
		typedef std::vector<std::shared_ptr<Viewport>> ViewportList;
		typedef std::vector<std::shared_ptr<RenderTarget>> RenderTargetList;
		typedef std::vector<std::shared_ptr<Mesh>> MeshList;
		typedef std::vector<std::shared_ptr<Texture>> TextureList;
		typedef std::vector<std::shared_ptr<Shader>> ShaderList;
	private: // Rendering Resource Storage
		BufferList mBuffers;
		MaterialList mMaterials;
		RenderStateList mRenderStates;
		SamplerStateList mSamplerStates;
		DepthStateList mDepthStates;
		BlendStateList mBlendStates;
		TextureList mTextures;
		ViewportList mViewports;
		RenderTargetList mRenderTargets;
		MeshList mMeshes;
		ShaderList mShaders;
	};
}