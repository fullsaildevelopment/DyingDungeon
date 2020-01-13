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

	void RenderPass::updatePerFrameBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, PerFrameBuffer& perFrame, Buffer* frameBuffer)
	{
		// Update and bind the constant buffer
		frameBuffer->updateData(context , &perFrame);
		frameBuffer->bind(context, 0, ShaderType::VertexShader);
		//shaderMatrixBuffer->bind(0, ShaderType::GeometryShader);
	}
	void RenderPass::updatePerObjectBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, PerObjectBuffer& perObject, Buffer* objectBuffer)
	{
		objectBuffer->updateData(context , &perObject);
		objectBuffer->bind(context, 1, ShaderType::VertexShader);
	}
}