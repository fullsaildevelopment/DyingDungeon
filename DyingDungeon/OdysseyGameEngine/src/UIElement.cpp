#include "UIElement.h"

namespace Odyssey
{
	const std::size_t UIElement::Type = std::hash<std::string>()(TO_STRING(Component));
	bool UIElement::isElementType(const std::size_t elementType) const
	{
		return elementType == Type;
	}
}