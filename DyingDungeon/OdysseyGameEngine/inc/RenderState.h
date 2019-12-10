#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderDevice;

	class RenderState
	{
	public:
		RenderState(RenderDevice& renderDevice, Topology topology, CullMode cullMode, FillMode fillMode, bool frontCCW = false, bool depthClipping = true, bool isShadowMap = false);
		void bind();
		~RenderState() = default;
	public:
		Topology getTopology();
	private:
		void convertToDirectX(Topology topology, CullMode cullMode, FillMode fillMode);
		Topology convertToCustom(D3D11_PRIMITIVE_TOPOLOGY topology);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
		D3D11_CULL_MODE mCullMode;
		D3D11_FILL_MODE mFillMode;
		bool mFrontCCW;
		bool mDepthClipping;
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
	};
}