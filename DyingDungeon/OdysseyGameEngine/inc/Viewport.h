#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderWindow;
	class RenderDevice;

	class Viewport
	{
	public: // Constructors
		Viewport(RenderDevice& renderDevice, RenderWindow* renderWindow);
		Viewport(RenderDevice& renderDevice, int width, int height, int topLeftX, int topLeftY, float minDepth, float maxDepth);
	public:
		void bind();
		void unbind();
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		D3D11_VIEWPORT mViewport;
		int mWidth;
		int mHeight;
		int mTopLeftX;
		int mTopLeftY;
		float mMinDepth;
		float mMaxDepth;
	};
}