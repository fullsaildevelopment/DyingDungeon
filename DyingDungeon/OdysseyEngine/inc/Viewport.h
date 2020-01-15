#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"

namespace Odyssey
{
	class RenderWindow;
	class RenderDevice;

	class Viewport
	{
	public: // Constructors
		Viewport(std::shared_ptr<RenderDevice> renderDevice, RenderWindow* renderWindow);
		Viewport(std::shared_ptr<RenderDevice> renderDevice, UINT width, UINT height, UINT topLeftX, UINT topLeftY, float minDepth, float maxDepth);
	public:
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	private:
		std::shared_ptr<RenderDevice> mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		D3D11_VIEWPORT mViewport;
		UINT mWidth;
		UINT mHeight;
		UINT mTopLeftX;
		UINT mTopLeftY;
		float mMinDepth;
		float mMaxDepth;
	};
}