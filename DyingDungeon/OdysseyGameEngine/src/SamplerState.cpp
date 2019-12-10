#include "SamplerState.h"
#include "RenderDevice.h"

namespace Odyssey
{
	SamplerState::SamplerState(RenderDevice& renderDevice, ComparisonFunc comparisonFunc, D3D11_FILTER filter, int bindSlot)
	{
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		mComparisonFunc = comparisonFunc;
		mBindSlot = bindSlot;

		D3D11_COMPARISON_FUNC dxComparisonFunc = D3D11_COMPARISON_NEVER;

		// Convert the comparison func enum to a directx 11 comparison func
		switch (mComparisonFunc)
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

		if (dxComparisonFunc != D3D11_COMPARISON_LESS_EQUAL)
		{
			// Create default sampler
			D3D11_SAMPLER_DESC samplerDesc = {};
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.Filter = filter;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = dxComparisonFunc;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			HRESULT hr = mDevice->CreateSamplerState(&samplerDesc, mSamplerState.GetAddressOf());
			assert(!FAILED(hr));
		}
		else
		{
			D3D11_SAMPLER_DESC comparisonSamplerDesc;
			ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
			comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.BorderColor[0] = 0.0f;
			comparisonSamplerDesc.BorderColor[1] = 0.0f;
			comparisonSamplerDesc.BorderColor[2] = 0.0f;
			comparisonSamplerDesc.BorderColor[3] = 0.0f;
			comparisonSamplerDesc.MinLOD = 0.f;
			comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			comparisonSamplerDesc.MipLODBias = 0.f;
			comparisonSamplerDesc.MaxAnisotropy = 0;
			comparisonSamplerDesc.ComparisonFunc = dxComparisonFunc;
			comparisonSamplerDesc.Filter = filter;
			HRESULT hr = mDevice->CreateSamplerState(&comparisonSamplerDesc, mSamplerState.GetAddressOf());
			assert(!FAILED(hr));
		}
	}

	void SamplerState::bind()
	{
		mDeviceContext->PSSetSamplers(mBindSlot, 1, mSamplerState.GetAddressOf());
	}

	void SamplerState::unbind()
	{
		mDeviceContext->PSSetSamplers(mBindSlot, 0, nullptr);
	}
}