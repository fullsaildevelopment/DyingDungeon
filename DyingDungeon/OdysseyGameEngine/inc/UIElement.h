#pragma once
#include "EngineIncludes.h"

#define TO_STRING( x ) #x

//****************
// CLASS_DECLARATION
//
// This macro must be included in the declaration of any subclass of Component.
// It declares variables used in type checking.
//****************
#define ELEMENT_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool isElementType( const std::size_t elementType ) const override;

//****************
// CLASS_DEFINITION
// 
// This macro must be included in the class definition to properly initialize 
// variables used in type checking. Take special care to ensure that the 
// proper parentclass is indicated or the run-time type information will be
// incorrect. Only works on single-inheritance RTTI.
//****************
#define ELEMENT_DEFINITION( parentclass, childclass )                                         \
const std::size_t childclass::Type = std::hash< std::string >()( TO_STRING( childclass ) ); \
bool childclass::isElementType( const std::size_t elementType ) const {                         \
        if ( elementType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::isElementType( elementType );                                       \
}  

namespace Odyssey
{
	class UIElement
	{
	public:
		static const std::size_t Type;
		virtual bool isElementType(const std::size_t elementType) const;
		virtual void draw(Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget) = 0;
		virtual ~UIElement() = default;
	protected:
		DirectX::XMFLOAT4 mColor;
	};
}