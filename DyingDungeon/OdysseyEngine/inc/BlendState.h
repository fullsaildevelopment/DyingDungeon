#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class BlendState
	{
	public:
		BlendState(BlendDesc& desc, float blendFactor[4]);
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	private:
		BlendDesc mBlendDesc;
		float mBlendFactor[4];
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
	};
}