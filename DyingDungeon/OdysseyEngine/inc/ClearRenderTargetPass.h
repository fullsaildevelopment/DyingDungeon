#pragma once
#include "EngineIncludes.h"
#include "RenderPass.h"

namespace Odyssey
{
	class RenderWindowDX11;

	class ClearRenderTargetPass : public RenderPass
	{
	public:
		ClearRenderTargetPass();
		virtual void render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, RenderWindowDX11* activeWindow, RenderArgs& args, RenderPackage& renderPackage);
	};
}