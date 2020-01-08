#include "Sprite2DPass.h"
#include "RenderDevice.h"
#include "RenderWindow.h"
#include "UICanvas.h"
#include "Entity.h"
#include "RenderWindowDX11.h"
#include "EventManager.h"

namespace Odyssey
{
	Sprite2DPass::Sprite2DPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> targetWindow)
	{
		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(targetWindow);
		mContext = renderDevice.getDevice2DContext();

		EventManager::getInstance().subscribe(this, &Sprite2DPass::onDebugEngine);
	}

	void Sprite2DPass::preRender(RenderArgs& args)
	{
		mContext->BeginDraw();
	}

	void Sprite2DPass::render(RenderArgs& args)
	{
		for (UIElement* element : args.elementList)
		{
			if (element->getCanvas()->isActive())
			{
				element->draw(mContext);
			}
		}

		if (mDebugCanvas)
		{
			for (UIElement* element : mDebugCanvas->getElements<UIElement>())
			{
				if (element->getCanvas()->isActive())
				{
					element->draw(mContext);
				}
			}
		}
		mContext->EndDraw();
	}

	void Sprite2DPass::onDebugEngine(DebugEngine* evnt)
	{
		mDebugCanvas = evnt->canvas;
	}
}