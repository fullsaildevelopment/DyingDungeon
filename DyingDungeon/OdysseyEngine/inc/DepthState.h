#pragma once
#include "RenderIncludes.h"
#include "EngineIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class DepthState
	{
	public:
		DepthState(bool depthWrite, bool depthRead, ComparisonFunc depthFunc);
		void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	private:
		void convertComparison(ComparisonFunc& customComparison, D3D11_COMPARISON_FUNC& dxComparisonFunc);
	private:
		bool mDepthWrite;
		bool mDepthRead;
		ComparisonFunc mDepthFunc;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
	};
}