#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderDevice;

	class SamplerState
	{
	public:
		SamplerState(RenderDevice& renderDevice, ComparisonFunc comparisonFunc, D3D11_FILTER filter, int bindSlot);
		void bind();
		void unbind();
		~SamplerState() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
		ComparisonFunc mComparisonFunc;
		int mBindSlot;
	};
}