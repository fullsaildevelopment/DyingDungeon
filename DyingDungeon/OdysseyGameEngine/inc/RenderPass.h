#pragma once
#include "EngineIncludes.h"

namespace Odyssey
{
	class Buffer;
	class Camera;
	class Light;
	class GameObject;

	struct RenderArgs
	{
		Camera* camera;
		ShaderMatrix shaderMatrix;
		std::shared_ptr<Buffer> shaderMatrixBuffer;
		std::vector<std::shared_ptr<GameObject>> renderList;
		std::vector<std::shared_ptr<GameObject>> transparentList;
		std::vector<std::shared_ptr<Light>> lightList;
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
		virtual void updateShaderMatrixBuffer(ShaderMatrix& shaderMatrix, std::shared_ptr<Buffer> shaderMatrixBuffer);
	private:
		bool mEnabled;
	};
}