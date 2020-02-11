#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "RenderTypes.h"

namespace Odyssey
{
	class Buffer;
	class Camera;
	class Light;
	class Entity;
	class MeshRenderer;
	class ParticleSystem;
	class RenderWindowDX11;
	class UICanvas;
	class UIElement;

	struct RenderArgs
	{
		PerFrameBuffer perFrame;
		Buffer* perFrameBuffer;
		PerObjectBuffer perObject;
		Buffer* perObjectBuffer;
		RenderWindowDX11* activeWindow;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		Microsoft::WRL::ComPtr< ID2D1DeviceContext> context2D;
	};

	class RenderPass
	{
	public:
		RenderPass();
		virtual ~RenderPass();
		virtual void setEnabled(bool enable);
		virtual bool isEnabled() const;
		virtual void preRender(RenderArgs& args, RenderPackage& renderPackage);
		virtual void render(RenderArgs& args, RenderPackage& renderPackage) = 0;
		virtual void postRender(RenderArgs& args, RenderPackage& renderPackage);
	protected:
		virtual void updatePerFrameBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, PerFrameBuffer& perFrame, Buffer* frameBuffer);
		virtual void updatePerObjectBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, PerObjectBuffer& perObject, Buffer* objectBuffer);
	private:
		bool mEnabled;
	};
}