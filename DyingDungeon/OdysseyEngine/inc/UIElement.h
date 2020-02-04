#pragma once
#include "EngineIncludes.h"
#include "RenderIncludes.h"
#include "EngineEvents.h"
#include "Callback.h"
#include "ReadWriteLock.h"

#pragma region Element Macros

#define TO_STRING( x ) #x

//****************
// CLASS_DECLARATION
//
// This macro must be included in the declaration of any subclass of Component.
// It declares variables used in type checking.
//****************
#define ELEMENT_DECLARATION( classname )														\
public:																							\
    static const std::size_t Type;																\
    virtual bool isElementType( const std::size_t elementType ) const override;

//****************
// CLASS_DEFINITION
// 
// This macro must be included in the class definition to properly initialize 
// variables used in type checking. Take special care to ensure that the 
// proper parentclass is indicated or the run-time type information will be
// incorrect. Only works on single-inheritance RTTI.
//****************
#define ELEMENT_DEFINITION( parentclass, childclass )											\
const std::size_t childclass::Type = std::hash< std::string >()( TO_STRING( childclass ) );		\
bool childclass::isElementType( const std::size_t elementType ) const {                         \
        if ( elementType == childclass::Type )													\
            return true;																		\
        return parentclass::isElementType( elementType );                                       \
}  

#pragma endregion

namespace Odyssey
{
	class UICanvas;

	class UIElement
	{
	public: // Type identifers
		static const std::size_t Type;
		virtual bool isElementType(const std::size_t elementType) const;

	public: // Rule of 3
		UIElement(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, UINT width, UINT height);
		~UIElement() = default;

	public:
		typedef void (UIElement::* MemberFunction)(void);
	public: // Events
		/**
		 *	Event callback to resize the UI element.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		virtual void onElementResize(UIElementResizeEvent* evnt);

		/**
		 *	Event callback to process a mouse click.
		 *	@param[in] evnt The event parameters.
		 *	@return void
		 */
		void onMouseClick(MouseClickEvent* evnt);

		void onMouseMove(MouseMoveEvent* evnt);

		void onMouseEnter();

		void onMouseExit();

		template<class T>
		void registerCallback(std::string function, T* instance, void(T::* memberFunction)())
		{
			mLock.lock(LockState::Write);
			mCallbackMap[function] = std::make_shared<CallbackHandler<T>>(instance, memberFunction);
			mLock.unlock(LockState::Write);
		}

		void unregisterCallback(std::string function)
		{
			mLock.lock(LockState::Write);
			if (mCallbackMap.count(function) != 0)
			{
				mCallbackMap.erase(function);
			}
			mLock.unlock(LockState::Write);
		}

	public: // Interface
		virtual void initialize();

		/**
		 *	Render the 2D UI element to the parameter render target.
		 *	@param[in] renderTarget The 2D render target to render the UI element to.
		 *	@return void
		 */
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context) = 0;

		void setCanvas(UICanvas* canvas);

		UICanvas* getCanvas();

		/**
		 *	Add to the element's position.
		 *	@param[in] x The x value to add to the position.
		 *	@param[in] y The y value to add to the position.
		 *	@return void
		 */
		void addPosition(float x, float y);

		/**
		 *	Set the element's position.
		 *	@param[in] x The x value of the position.
		 *	@param[in] y The y value of the position.
		 *	@return void
		 */
		void setPosition(float x, float y);

		/**
		 *	Get the element's position.
		 *	@param[in] void
		 *	@return XMFLOAT2 The element's position.
		 */
		DirectX::XMFLOAT2 getPosition();

		/**
		 *	Add to the element's scale.
		 *	@param[in] x The x value to add to the scale.
		 *	@param[in] y The y value to add to the scale.
		 *	@return void
		 */
		void addScale(float x, float y);

		/**
		 *	Set the element's scale.
		 *	@param[in] x The element's x scale.
		 *	@param[in] y The element's y scale.
		 *	@return void
		 */
		void setScale(float x, float y);

		/**
		 *	Get the element's scale.
		 *	@param[in] void
		 *	@return XMFLOAT2 The element's scale.
		 */
		DirectX::XMFLOAT2 getScale();

		/**
		 *	Set the element's width and height.
		 *	@param[in] width The element's width.
		 *	@param[in] height The element's height.
		 *	@return void
		 */
		void setDimensions(float width, float height);

		/**
		 *	Get the element's width and height.
		 *	@param[in] void
		 *	@return XMFloat2 The element's width and height.
		 */
		DirectX::XMFLOAT2 getDimensions();

		/**
		 *	Add to the element's color.
		 *	@param[in] color The color to add to the UI element's color. (0 - 255)
		 *	@return void
		 */
		void addColor(DirectX::XMFLOAT3 color);

		/**
		 *	Add to the element's color channels.
		 *	@param[in] r The color's red channel. (0 - 255)
		 *	@param[in] g The color's green channel. (0 - 255)
		 *	@param[in] b The color's blue channel. (0 - 255)
		 *	@return void
		 */
		void addColor(float r, float g, float b);

		/**
		 *	Set the element's color.
		 *	@param[in] color The UI element's color. (0 - 255)
		 *	@return void
		 */
		void setColor(DirectX::XMFLOAT3 color);

		/**
		 *	Set the element's color channels.
		 *	@param[in] r The color's red channel. (0 - 255)
		 *	@param[in] g The color's green channel. (0 - 255)
		 *	@param[in] b The color's blue channel. (0 - 255)
		 *	@return void
		 */
		void setColor(float r, float g, float b);

		/**
		 *	Get the element's color with values 0 - 255.
		 *	@param[in] void
		 *	@return XMFloat4 The UI element's color.
		 */
		DirectX::XMFLOAT4 getColor();

		/**
		 *	Add to the element's opacity.
		 *	@param[in] opacity The value to add to the opacity.  (0.0 - 1.0)
		 *	@return void
		 */
		void addOpacity(float opacity);

		/**
		 *	Set the element's opacity.
		 *	@param[in] opacity The element's opacity. (0.0 - 1.0)
		 *	@return void
		 */
		void setOpacity(float opacity);

		/**
		 *	Get the element's opacity with a value 0.0 - 1.0.
		 *	@param[in] void
		 *	@return float The element's opacity.
		 */
		float getOpacity();

		void addFill(float value);

		void setFill(float fill);

		void setVisible(bool visible);

		bool isVisible();
	protected:
		/**
		 *	Clamp the color channels between 0.0 and 1.0
		 *	@param[out] color The color to clamp
		 *	@return void
		 */
		void clampColor(DirectX::XMFLOAT4& color);

		/**
		 *	Create the element's shape using the position and dimensions.
		 *	@param[in] void
		 *	@return void
		 */
		void createShape();

		virtual void createResource();

		void resetBrush();

	private:
		bool mTrackMouseEnter;
		bool mTrackMouseExit;

	protected: // Members
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mBrush;
		D2D_RECT_F mShape;
		DirectX::XMFLOAT2 mPosition;
		DirectX::XMFLOAT2 mScale;
		DirectX::XMFLOAT2 mDimensions;
		DirectX::XMFLOAT4 mColor;
		DirectX::XMFLOAT2 mScreenScale;
		UICanvas* mCanvas;
		std::map<std::string, std::shared_ptr<AbstractCallbackHandler>> mCallbackMap;
		ReadWriteLock mLock;
		float mFill;
		bool mIsVisible;
		bool mIsRegistered;
	};
}