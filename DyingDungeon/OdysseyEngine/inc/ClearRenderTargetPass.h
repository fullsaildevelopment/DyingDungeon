#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderWindowDX11;
	class RenderDevice;

	class ClearRenderTargetPass : public RenderPass
	{
	public:
		ClearRenderTargetPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<RenderWindow> renderWindow, bool clearDepth);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage);
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		std::shared_ptr<RenderDevice> mRenderDevice;
		std::shared_ptr<RenderWindowDX11> mRenderWindow;
		bool mClearDepth;
	};
}