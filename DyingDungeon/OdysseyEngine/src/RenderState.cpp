#include "RenderState.h"
#include "RenderManager.h"

namespace Odyssey
{
	RenderState::RenderState(Topology topology, CullMode cullMode, FillMode fillMode, bool frontCCW, bool depthClipping, bool isShadowMap) :
		mFrontCCW(frontCCW), mDepthClipping(depthClipping)
	{
		// Get the device and immediate context from the render manager
		mDevice = RenderManager::getInstance().getDX11Device();

		// Convert the custom enums to directX enums
		convertToDirectX(topology, cullMode, fillMode);

		// Create the rasterizer state based on the input
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FrontCounterClockwise = mFrontCCW;
		rasterizerDesc.DepthClipEnable = mDepthClipping;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FillMode = mFillMode;
		rasterizerDesc.AntialiasedLineEnable = true;
		rasterizerDesc.MultisampleEnable = true;
		rasterizerDesc.DepthBias = 10000;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias = 1.0f;

		if (isShadowMap)
		{
			rasterizerDesc.DepthBias = 10000;
			rasterizerDesc.DepthBiasClamp = 0.0f;
			rasterizerDesc.SlopeScaledDepthBias = 1.0f;
		}

		HRESULT hr = mDevice->CreateRasterizerState(&rasterizerDesc, mRasterizerState.GetAddressOf());
		assert(!FAILED(hr));
	}

	void RenderState::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->IASetPrimitiveTopology(mTopology);
		context->RSSetState(mRasterizerState.Get());
	}

	Topology RenderState::getTopology()
	{
		return convertToCustom(mTopology);
	}

	void RenderState::convertToDirectX(Topology topology, CullMode cullMode, FillMode fillMode)
	{
		switch (topology)
		{
		case Topology::PointList:
		{
			mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		}
		case Topology::LineList:
		{
			mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		}
		case Topology::LineStrip:
		{
			mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			break;
		}
		case Topology::TriangleList:
		{
			mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		}
		}

		switch (cullMode)
		{
		case CullMode::CULL_NONE:
		{
			mCullMode = D3D11_CULL_NONE;
			break;
		}
		case CullMode::CULL_BACK:
		{
			mCullMode = D3D11_CULL_BACK;
			break;
		}
		case CullMode::CULL_FRONT:
		{
			mCullMode = D3D11_CULL_FRONT;
			break;
		}
		}

		switch (fillMode)
		{
			case FillMode::FILL_SOLID:
			{
				mFillMode = D3D11_FILL_SOLID;
				break;
			}
			case FillMode::FILL_WIREFRAME:
			{
				mFillMode = D3D11_FILL_WIREFRAME;
				break;
			}
		}
	}

	Topology RenderState::convertToCustom(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		Topology retTopology;
		switch (topology)
		{
			case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:
			{
				retTopology = Topology::PointList;
				break;
			}
			case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
			{
				retTopology = Topology::LineList;
				break;
			}
			case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
			{
				retTopology = Topology::LineStrip;
				break;
			}
			case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
			{
				retTopology = Topology::TriangleList;
				break;
			}
		}
		return retTopology;
	}
}