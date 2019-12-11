#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"
#include "Light.h"
namespace Odyssey
{
	class AABB;
	class RenderTarget;
	class RenderState;
	class Shader;
	class MeshRenderer;
	class RenderDevice;

	struct SceneLighting
	{
		Light sceneLights[8];
		///////////////////////////// 16 x 10 bytes
		unsigned int numLights;
		DirectX::XMFLOAT3 camPos;
		///////////////////////////// 16 bytes
	};

	class OpaquePass : public RenderPass
	{
	public:
		OpaquePass(RenderDevice& renderDevice, std::shared_ptr<RenderTarget> renderTarget);
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
		void setFrustumCullEnable(bool enable);
	private:
		void renderSceneObject(std::shared_ptr<GameObject> object, RenderArgs& args);
		void setupLightingBuffer(AABB* objectAABB, RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<RenderTarget> mRenderTarget;
		std::shared_ptr<RenderState> mRenderState;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
		std::shared_ptr<Buffer> mLightingBuffer;
		bool mFrustumCull;
	};

	struct Cmp
	{
		bool operator()(const float& v1, const float& v2) const
		{
			return (v1 < v2);
		}
	};
}