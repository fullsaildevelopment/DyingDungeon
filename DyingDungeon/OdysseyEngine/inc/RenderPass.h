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
	class RenderWindow;
	class UICanvas;
	class UIElement;

	struct RenderArgs
	{
		Entity* camera;
		DirectX::XMFLOAT3 camPos;
		PerFrameBuffer perFrame;
		Buffer* perFrameBuffer;
		PerObjectBuffer perObject;
		Buffer* perObjectBuffer;
		DirectX::XMFLOAT3 sceneCenter;
		float sceneRadius;
		std::shared_ptr<Light> shadowLight;
		Entity* skybox;
		std::vector<std::shared_ptr<Entity>> entityList;
		std::vector<std::shared_ptr<Entity>> transparentList;
		std::vector<std::shared_ptr<Light>> lightList;
		std::vector<UIElement*> elementList;
		std::vector<MeshRenderer*> renderList;
		std::vector<ParticleSystem*> systemList;
	};

	class RenderPass
	{
	public:
		RenderPass();
		virtual ~RenderPass();
		virtual void setEnabled(bool enable);
		virtual bool isEnabled() const;
		virtual void preRender(RenderArgs& args);
		virtual void render(RenderArgs& args) = 0;
		virtual void postRender(RenderArgs& args);
	protected:
		virtual void updatePerFrameBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, PerFrameBuffer& perFrame, Buffer* frameBuffer);
		virtual void updatePerObjectBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, PerObjectBuffer& perObject, Buffer* objectBuffer);
	private:
		bool mEnabled;
	};
}