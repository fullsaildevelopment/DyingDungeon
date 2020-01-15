#pragma once
#include "EngineIncludes.h"
#include "RenderTypes.h"
#include "EngineEvents.h"
#include "UIElement.h"

namespace Odyssey
{
	class Text2D : public UIElement
	{
		ELEMENT_DECLARATION(Text2D)

	public: // Rule of 3
		Text2D(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, UINT width, UINT height, std::wstring text, TextProperties textProperties);

	public: // Events
		/**
		 *	Event callback to resize the text element
		 *	@param[in] evnt The event parameters
		 *	@return void
		 */
		void onTextResize(UIElementResizeEvent* evnt);

	public: // Interface
		/**
		 *	Render the 2D text to the parameter render target.
		 *	@param[in] renderTarget The 2D render target to render the text to.
		 *	@return void
		 */
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);

		/**
		 *	Set the text element to be rendered.
		 *	@param[in] text The text element to be rendered.
		 *	@return void
		 */
		void setText(std::wstring text);

		/**
		 *	Set the text's text alignment.
		 *	@param[in] alignment The text alignment.
		 *	@return void
		 */
		void setTextAlignment(TextAlignment alignment);

		/**
		 *	Set the text's paragraph alignment.
		 *	@param[in] alignment The paragraph alignment.
		 *	@return void
		 */
		void setParagraphAlignment(ParagraphAlignment alignment);

		/**
		 *	Set the text's font name.
		 *	@param[in] fontName The name of the font.
		 *	@return void
		 */
		void setFont(std::wstring fontName);

		/**
		 *	Set the text's font size.
		 *	@param[in] fontSize The size of the font.
		 *	@return void
		 */
		void setFontSize(float fontSize);

		/**
		 *	Set the text's bold style flag.
		 *	@param[in] bold Flag whether the text uses the bold style.
		 *	@return void
		 */
		void setBoldStyle(bool bold);

		/**
		 *	Set the text's italic style flag.
		 *	@param[in] italic Flag whether the text uses the italic style.
		 *	@return void
		 */
		void setItalicStyle(bool italic);

	private: // Helpers
		/**
		 *	Create the text format resource from a TextProperties object
		 *	@param[in] textProperties The text properties object to create the text format from.
		 *	@return void
		 */
		void createTextFormat(TextProperties textProperties);

		/**
		 *	Convert the custom text properties struct into Direct2D equivalents.
		 *	@param[in] textProperties The text properties object to convert.
		 *	@param[out] fontWeight The converted font weight object.
		 *	@param[out] fontStyle The converted font style object.
		 *	@param[out] textAlignment The converted text alignment object.
		 *	@param[out] paragraphAlignment The converted paragraph alignment object.
		 *	@return void
		 */
		void convertTextProperties(TextProperties textProperties, DWRITE_FONT_WEIGHT& fontWeight, DWRITE_FONT_STYLE& fontStyle, DWRITE_TEXT_ALIGNMENT& textAlignment, DWRITE_PARAGRAPH_ALIGNMENT& paragraphAlignment);
	
	private: // Members
		Microsoft::WRL::ComPtr<IDWriteFactory> mFactory;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> mFormat;
		TextProperties mProperties;
		std::wstring mText;
	};
}