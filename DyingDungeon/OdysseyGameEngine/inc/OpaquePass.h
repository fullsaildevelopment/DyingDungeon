#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderTarget;
	class RenderState;
	class Shader;
	class MeshRenderer;
	class RenderDevice;

	class OpaquePass : public RenderPass
	{
	public:
		OpaquePass(RenderDevice& renderDevice, std::shared_ptr<RenderTarget> renderTarget);
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args);
		void setFrustumCullEnable(bool enable);
	private:
		void renderSceneObject(std::shared_ptr<GameObject> object, RenderArgs& args);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		std::shared_ptr<RenderTarget> mRenderTarget;
		std::shared_ptr<RenderState> mRenderState;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mPixelShader;
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