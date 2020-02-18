#include "CharacterHUDElements.h"

CLASS_DEFINITION(Component, CharacterHUDElements)

std::shared_ptr<Odyssey::Component> CharacterHUDElements::clone() const
{
	return std::make_shared<CharacterHUDElements>(*this);
}

void CharacterHUDElements::initialize()
{
}

void CharacterHUDElements::update(double deltaTime)
{
}

void CharacterHUDElements::onDestroy()
{
}

void CharacterHUDElements::ChangeSkill(Skills* _skill, int _numOfSkill)
{
	switch (_numOfSkill)
	{
		case 1:
			// Set the new sprite image
			pSkill1->setSprite(_skill->GetSkillIconPath(), pSkill1->getDimensions().x, pSkill1->getDimensions().y);
			break;
		case 2:
			// Set the new sprite image
			pSkill2->setSprite(_skill->GetSkillIconPath(), pSkill2->getDimensions().x, pSkill2->getDimensions().y);
			break;
		case 3:
			// Set the new sprite image
			pSkill3->setSprite(_skill->GetSkillIconPath(), pSkill3->getDimensions().x, pSkill3->getDimensions().y);
			break;
		case 4:
			// Set the new sprite image
			pSkill4->setSprite(_skill->GetSkillIconPath(), pSkill4->getDimensions().x, pSkill4->getDimensions().y);
			break;
		default:
			break;
	}
}
