#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class SamplerState
	{
	public:
		SamplerState(ComparisonFunc comparisonFunc, D3D11_FILTER filter, int bindSlot);
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		~SamplerState() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
		ComparisonFunc mComparisonFunc;
		int mBindSlot;
	};
}