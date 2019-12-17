#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "UIElement.h"

namespace Odyssey
{
	class RenderDevice;

	class UICanvas : public Component
	{
		CLASS_DECLARATION(UICanvas)
	public:
		UICanvas();

	public:
		/**
		 *	Initialize the UICanvas with default values. Called once when the scene is loaded.
		 *	@param[in] void
		 *	@return void
		 */
		virtual void initialize();

		/**
		 *	Set the UICanvas's active state.
		 *	@param[in] active The new active state of the UICanvas.
		 *	@return void
		 */
		void setActive(bool active);

		/**
		 *	Get the UICanvas's active state.
		 *	@param[in] void
		 *	@return bool The active state of the UICanvas.
		 */
		bool getActive();

		void addUIElement(std::shared_ptr<UIElement> element);
		void addSprite2D(DirectX::XMFLOAT2 position, LPCWSTR filename, float width = 0.0f, float height = 0.0f);
		void addRectangle2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, float width, float height);
		std::vector<std::shared_ptr<UIElement>> getUIElements();
	private:
		std::vector< std::shared_ptr<UIElement>> mUIElements;
		bool mIsActive;
	};
}