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
			pSkill1->setSprite(_skill->GetSkillIconPath(), (UINT)pSkill1->getDimensions().x, (UINT)pSkill1->getDimensions().y);
			break;
		case 2:
			// Set the new sprite image
			pSkill2->setSprite(_skill->GetSkillIconPath(), (UINT)pSkill2->getDimensions().x, (UINT)pSkill2->getDimensions().y);
			break;
		case 3:
			// Set the new sprite image
			pSkill3->setSprite(_skill->GetSkillIconPath(), (UINT)pSkill3->getDimensions().x, (UINT)pSkill3->getDimensions().y);
			break;
		case 4:
			// Set the new sprite image
			pSkill4->setSprite(_skill->GetSkillIconPath(), (UINT)pSkill4->getDimensions().x, (UINT)pSkill4->getDimensions().y);
			break;
		default:
			break;
	}
}
