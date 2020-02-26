#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"

namespace Odyssey
{
	class RenderWindow;

	class Viewport
	{
	public: // Constructors
		Viewport(RenderWindow* renderWindow);
		Viewport(UINT width, UINT height, UINT topLeftX, UINT topLeftY, float minDepth, float maxDepth);
	public:
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	private:
		D3D11_VIEWPORT mViewport;
		UINT mWidth;
		UINT mHeight;
		UINT mTopLeftX;
		UINT mTopLeftY;
		float mMinDepth;
		float mMaxDepth;
	};
}