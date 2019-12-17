#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class UIElement
	{
	public:
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget) = 0;
	protected:
		DirectX::XMFLOAT4 mColor;
	};
}