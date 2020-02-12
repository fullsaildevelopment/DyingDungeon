#include "DepthState.h"
#include "RenderManager.h"

namespace Odyssey
{
	DepthState::DepthState(bool depthWrite, bool depthRead, ComparisonFunc depthFunc)
	{
		mDepthWrite = depthWrite;
		mDepthRead = depthRead;
		mDepthFunc = depthFunc;

		D3D11_COMPARISON_FUNC compFunc;
		convertComparison(mDepthFunc, compFunc);

		D3D11_DEPTH_STENCIL_DESC depthDesc;
		depthDesc.DepthEnable = mDepthRead;
		depthDesc.DepthWriteMask = (mDepthWrite) ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDesc.DepthFunc = compFunc;
		depthDesc.StencilEnable = mDepthWrite;
		depthDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		// Stencil operations if pixel is front-facing
		depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = RenderManager::getInstance().getDX11Device()->CreateDepthStencilState(&depthDesc, mDepthStencilState.GetAddressOf());
		assert(!FAILED(hr));
	}

	void DepthState::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	}

	void DepthState::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->OMSetDepthStencilState(nullptr, 0);
	}

	void DepthState::convertComparison(ComparisonFunc& customComparison, D3D11_COMPARISON_FUNC& dxComparisonFunc)
	{
		switch (customComparison)
		{
		case ComparisonFunc::COMPARISON_NEVER:
		{
			dxComparisonFunc = D3D11_COMPARISON_NEVER;
			break;
		}
		case ComparisonFunc::COMPARISON_ALWAYS:
		{
			dxComparisonFunc = D3D11_COMPARISON_ALWAYS;
			break;
		}
		case ComparisonFunc::COMPARISON_EQUAL:
		{
			dxComparisonFunc = D3D11_COMPARISON_EQUAL;
			break;
		}
		case ComparisonFunc::COMPARISON_NOT_EQUAL:
		{
			dxComparisonFunc = D3D11_COMPARISON_NOT_EQUAL;
			break;
		}
		case ComparisonFunc::COMPARISON_GREATER:
		{
			dxComparisonFunc = D3D11_COMPARISON_GREATER;
			break;
		}
		case ComparisonFunc::COMPARISON_GREATER_EQUAL:
		{
			dxComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
			break;
		}
		case ComparisonFunc::COMPARISON_LESS:
		{
			dxComparisonFunc = D3D11_COMPARISON_LESS;
			break;
		}
		case ComparisonFunc::COMPARISON_LESS_EQUAL:
		{
			dxComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
			break;
		}
		}
	}
}