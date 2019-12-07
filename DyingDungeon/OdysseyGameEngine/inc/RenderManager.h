#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class RenderManager
	{
	public: // Public Singleton Implementation
		static RenderManager& getInstance();
		~RenderManager() { }
	private: // Private Constructor
		RenderManager() { }
	public:
		void initialize(HWND& hWnd, Microsoft::WRL::ComPtr<IDXGISwapChain>& outSwapChain);
		Microsoft::WRL::ComPtr<ID3D11Device> getDevice();
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	};
}