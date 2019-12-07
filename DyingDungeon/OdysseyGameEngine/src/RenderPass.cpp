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

	void RenderPass::updateShaderMatrixBuffer(ShaderMatrix& shaderMatrix, std::shared_ptr<Buffer> shaderMatrixBuffer)
	{
		// Calculate the WVP matrix
		DirectX::XMMATRIX wvp = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&shaderMatrix.world), DirectX::XMLoadFloat4x4(&shaderMatrix.view));
		// Multiply in the projection for the MVP matrix
		wvp = DirectX::XMMatrixMultiply(wvp, DirectX::XMLoadFloat4x4(&shaderMatrix.proj));
		DirectX::XMStoreFloat4x4(&shaderMatrix.worldViewProj, wvp);

		// Update and bind the constant buffer
		shaderMatrixBuffer->updateData(&shaderMatrix);
		shaderMatrixBuffer->bind(0, ShaderType::VertexShader);
		shaderMatrixBuffer->bind(0, ShaderType::GeometryShader);
	}
}