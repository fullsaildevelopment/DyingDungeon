#include "BlendState.h"
#include "RenderManager.h"

namespace Odyssey
{
	BlendState::BlendState(BlendDesc& blendDesc, float blendFactor[4])
	{
		mBlendDesc = blendDesc;

		for (int i = 0; i < 4; i++)
		{
			mBlendFactor[i] = blendFactor[i];
		}
		D3D11_BLEND_DESC dx11BlendDesc;
		ZeroMemory(&dx11BlendDesc, sizeof(dx11BlendDesc));

		D3D11_RENDER_TARGET_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BlendEnable = mBlendDesc.mBlendEnable;
		desc.SrcBlend = (D3D11_BLEND)mBlendDesc.mSrcBlend;
		desc.DestBlend = (D3D11_BLEND)mBlendDesc.mDestBlend;
		desc.BlendOp = (D3D11_BLEND_OP)mBlendDesc.mBlendOp;
		desc.SrcBlendAlpha = (D3D11_BLEND)mBlendDesc.mSrcAlphaBlend;
		desc.DestBlendAlpha = (D3D11_BLEND)mBlendDesc.mDestAlphaBlend;
		desc.BlendOpAlpha = (D3D11_BLEND_OP)mBlendDesc.mAlphaBlendOp;
		desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		dx11BlendDesc.AlphaToCoverageEnable = mBlendDesc.mAlphaToCoverage;
		dx11BlendDesc.IndependentBlendEnable = mBlendDesc.mIndependentBlendEnable;
		dx11BlendDesc.RenderTarget[0] = desc;

		HRESULT hr =RenderManager::getInstance().getDX11Device()->CreateBlendState(&dx11BlendDesc, mBlendState.GetAddressOf());
		assert(!FAILED(hr));
	}

	void BlendState::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		context->OMSetBlendState(mBlendState.Get(), mBlendFactor, 0xffffffff);
	}

	void BlendState::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		float blend[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		context->OMSetBlendState(nullptr, blend, 0xffffffff);
	}
}