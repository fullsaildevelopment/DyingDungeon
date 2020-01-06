#include "Sprite2DPass.h"
#include "RenderDevice.h"
#include "RenderWindow.h"
#include "UICanvas.h"
#include "Entity.h"
#include "RenderWindowDX11.h"

namespace Odyssey
{
	Sprite2DPass::Sprite2DPass(RenderDevice& renderDevice, std::shared_ptr<RenderWindow> targetWindow)
	{
		mRenderWindow = std::static_pointer_cast<RenderWindowDX11>(targetWindow);
	}

	void Sprite2DPass::preRender(RenderArgs& args)
	{
		mRenderWindow->get2DRenderTarget()->BeginDraw();
	}

	void Sprite2DPass::render(RenderArgs& args)
	{
		for (UICanvas* canvas : args.canvasList)
		{
			for (auto element : canvas->getElements<UIElement>())
			{
				element->draw(mRenderWindow->get2DRenderTarget());
			}
		}
		mRenderWindow->get2DRenderTarget()->EndDraw();
	}
}