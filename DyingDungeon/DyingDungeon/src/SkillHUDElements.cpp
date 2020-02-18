#include "SkillHUDElements.h"

CLASS_DEFINITION(Component, SkillHUDElements)

std::shared_ptr<Odyssey::Component> SkillHUDElements::clone() const
{
	return std::make_shared<SkillHUDElements>(*this);
}

void SkillHUDElements::initialize()
{
}

void SkillHUDElements::update(double deltaTime)
{
}

void SkillHUDElements::onDestroy()
{
}

void SkillHUDElements::ChangeThemeColor(DirectX::XMFLOAT3 _themeColor)
{
	mSkillName->setColor(_themeColor);
	mManaCost->setColor(_themeColor);
	mSkillDescription->setColor(_themeColor);
}
