#include "TransparentPass.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "Entity.h"
#include "Transform.h"
#include "RenderDevice.h"
#include "Mesh.h"
#include "ParticleSystem.h"
#include "MeshRenderer.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	TransparentPass::TransparentPass(std::shared_ptr<RenderDevice> renderDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<RenderWindow> renderWindow)
	{
		mDeviceContext = context;
		mRenderDevice = renderDevice;
		mDevice = renderDevice->getDevice();

		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(renderWindow);
	}

	void TransparentPass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Update the buffer
		updatePerFrameBuffer(mDeviceContext, args.perFrame, args.perFrameBuffer);
		args.perFrameBuffer->bind(mDeviceContext, 0, ShaderType::GeometryShader);

		mRenderWindow->get3DRenderTarget()->bind(mDeviceContext);
	}

	void TransparentPass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Bind the world
		for (VFXObject vfxObject : renderPackage.vfxObjects)
		{
			if (vfxObject.system->isActive() && vfxObject.system->getEntity()->isActive() && vfxObject.system->getEntity()->isVisible())
			{
				args.perObject.world = vfxObject.transform->getGlobalTransform(true);
				updatePerObjectBuffer(mDeviceContext, args.perObject, args.perObjectBuffer);
				args.perObjectBuffer->bind(mDeviceContext, 1, ShaderType::GeometryShader);

				vfxObject.system->run(mDeviceContext);
			}
		}
	}
}