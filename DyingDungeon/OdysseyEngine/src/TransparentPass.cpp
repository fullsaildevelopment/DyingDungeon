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
#include "RenderManager.h"
#include "BlendState.h"

namespace Odyssey
{
	TransparentPass::TransparentPass()
	{
		// Create the blend state
		float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		BlendDesc blendDesc;
		blendDesc.mSrcBlend = Blend::BLEND_SRC_ALPHA;
		blendDesc.mDestBlend = Blend::BLEND_ONE;
		blendDesc.mBlendOp = BlendOperation::BLEND_OP_ADD;
		blendDesc.mSrcAlphaBlend = Blend::BLEND_ZERO;
		blendDesc.mDestAlphaBlend = Blend::BLEND_ONE;
		blendDesc.mAlphaBlendOp = BlendOperation::BLEND_OP_ADD;
		blendDesc.mAlphaToCoverage = false;
		blendDesc.mIndependentBlendEnable = false;
		mBlendState = RenderManager::getInstance().createBlendState(blendDesc, blendFactor);
	}

	void TransparentPass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		// Update the buffer
		updatePerFrameBuffer(args.context, args.perFrame, args.perFrameBuffer);
		args.perFrameBuffer->bind(args.context, 0, ShaderType::GeometryShader);

		args.activeWindow->get3DRenderTarget()->bind(args.context);
	}

	void TransparentPass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		//RenderManager::getInstance().getBlendState(mBlendState)->bind(args.context);

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
		//RenderManager::getInstance().getBlendState(mBlendState)->unbind(args.context);
	}
}