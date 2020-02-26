#include "Sprite2DPass.h"
#include "RenderWindow.h"
#include "UICanvas.h"
#include "Entity.h"
#include "RenderWindowDX11.h"
#include "EventManager.h"

namespace Odyssey
{
	Sprite2DPass::Sprite2DPass()
	{
		EventManager::getInstance().subscribe(this, &Sprite2DPass::onDebugEngine);
	}

	void Sprite2DPass::preRender(RenderArgs& args, RenderPackage& renderPackage)
	{
		args.context2D->BeginDraw();
	}

	void Sprite2DPass::render(RenderArgs& args, RenderPackage& renderPackage)
	{
		for (CanvasObject canvas : renderPackage.canvasObjects)
		{
			if (canvas.canvas->isActive() && canvas.entity->isActive() && canvas.entity->isVisible())
			{
				for (UIElement* element : canvas.elements)
				{
					if (element->isVisible())
					{
						element->draw(args.context2D);
					}
				}
			}
		}

		if (mDebugCanvas)
		{
			for (UIElement* element : mDebugCanvas->getElements<UIElement>())
			{
				if (element->getCanvas()->isActive())
				{
					element->draw(args.context2D);
				}
			}
		}
		args.context2D->EndDraw();
	}

	void Sprite2DPass::onDebugEngine(DebugEngine* evnt)
	{
		mDebugCanvas = evnt->canvas;
	}
}