#include "RenderPass.h"
#include "Buffer.h"

namespace Odyssey
{
	RenderPass::RenderPass()
	{
		mEnabled = true;
	}

	RenderPass::~RenderPass()
	{
	}

	void RenderPass::setEnabled(bool enable)
	{
		mEnabled = enable;
	}

	bool RenderPass::isEnabled() const
	{
		return mEnabled;
	}

	void RenderPass::preRender(RenderArgs& args)
	{
	}

	void RenderPass::postRender(RenderArgs& args)
	{
	}

	void RenderPass::updatePerFrameBuffer(PerFrameBuffer& perFrame, Buffer* frameBuffer)
	{
		// Update and bind the constant buffer
		frameBuffer->updateData(&perFrame);
		frameBuffer->bind(0, ShaderType::VertexShader);
		//shaderMatrixBuffer->bind(0, ShaderType::GeometryShader);
	}
	void RenderPass::updatePerObjectBuffer(PerObjectBuffer& perObject, Buffer* objectBuffer)
	{
		objectBuffer->updateData(&perObject);
		objectBuffer->bind(1, ShaderType::VertexShader);
	}
}