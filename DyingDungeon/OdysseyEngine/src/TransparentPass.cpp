#include "TransparentPass.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "RenderState.h"
#include "Entity.h"
#include "Transform.h"
#include "Mesh.h"
#include "ParticleSystem.h"
#include "MeshRenderer.h"
#include "RenderWindow.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	void TransparentPass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Update the buffer
		updatePerFrameBuffer(args.context, args.perFrame, args.perFrameBuffer);
		args.perFrameBuffer->bind(args.context, 0, ShaderType::GeometryShader);

		args.activeWindow->get3DRenderTarget()->bind(args.context);
	}

	void TransparentPass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Bind the world
		for (VFXObject vfxObject : renderPackage.vfxObjects)
		{
			if (vfxObject.system->isActive() && vfxObject.system->getEntity()->isActive() && vfxObject.system->getEntity()->isVisible())
			{
				args.perObject.world = vfxObject.transform->getGlobalTransform(true);
				updatePerObjectBuffer(args.context, args.perObject, args.perObjectBuffer);
				args.perObjectBuffer->bind(args.context, 1, ShaderType::GeometryShader);

				vfxObject.system->run(args.context);
			}
		}
	}
}