#pragma once
#include "EngineIncludes.h"
#include "RenderTypes.h"
#include "RenderPass.h"
#include "Light.h"

namespace Odyssey
{
	class RenderWindowDX11;
	class RenderState;
	class Shader;
	class MeshRenderer;
	class RenderDevice;

	class OpaquePass : public RenderPass
	{
		struct SceneLighting
		{
			ShaderLight sceneLights[8];
			///////////////////////////// 16 x 8 bytes
			unsigned int numLights;
			DirectX::XMFLOAT3 camPos;
			///////////////////////////// 16 bytes
		};

	public:
		OpaquePass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<RenderWindow> renderWindow);
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
		void setFrustumCullEnable(bool enable);
	private:
		void updateLightingBuffer(RenderObject* renderObject, RenderArgs& args, RenderPackage& renderPackage);
		void renderSceneObject(RenderObject* renderObject, RenderArgs& args);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<RenderWindowDX11> mRenderWindow;
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