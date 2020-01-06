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
		Viewport(RenderDevice& renderDevice, RenderWindow* renderWindow);
		Viewport(RenderDevice& renderDevice, UINT width, UINT height, UINT topLeftX, UINT topLeftY, float minDepth, float maxDepth);
	public:
		void bind();
		void unbind();
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		D3D11_VIEWPORT mViewport;
		UINT mWidth;
		UINT mHeight;
		UINT mTopLeftX;
		UINT mTopLeftY;
		float mMinDepth;
		float mMaxDepth;
	};
}