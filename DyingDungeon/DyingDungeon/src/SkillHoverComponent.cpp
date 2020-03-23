#include "SkillHoverComponent.h"
#include "SkillHUDElements.h"
#include "EventManager.h"
#include "Character.h"

CLASS_DEFINITION(Odyssey::Component, SkillHoverComponent)

std::shared_ptr<Odyssey::Component> SkillHoverComponent::clone() const 
{
	return std::make_shared<SkillHoverComponent>(*this);
}

void SkillHoverComponent::initialize()
{
	// Enter callbacks
	mSprites[0]->registerCallback("onMouseEnter", this, &SkillHoverComponent::skill1Enter);
	mSprites[1]->registerCallback("onMouseEnter", this, &SkillHoverComponent::skill2Enter);
	mSprites[2]->registerCallback("onMouseEnter", this, &SkillHoverComponent::skill3Enter);
	mSprites[3]->registerCallback("onMouseEnter", this, &SkillHoverComponent::skill4Enter);
	// Exit callbacks
	mSprites[0]->registerCallback("onMouseExit", this, &SkillHoverComponent::skill1Exit);
	mSprites[1]->registerCallback("onMouseExit", this, &SkillHoverComponent::skill2Exit);
	mSprites[2]->registerCallback("onMouseExit", this, &SkillHoverComponent::skill3Exit);
	mSprites[3]->registerCallback("onMouseExit", this, &SkillHoverComponent::skill4Exit);
}

void SkillHoverComponent::onDestroy()
{
	// Enter callbacks
	mSprites[0]->unregisterCallback("onMouseEnter");
	mSprites[1]->unregisterCallback("onMouseEnter");
	mSprites[2]->unregisterCallback("onMouseEnter");
	mSprites[3]->unregisterCallback("onMouseEnter");
	// Exit callbacks
	mSprites[0]->unregisterCallback("onMouseExit");
	mSprites[1]->unregisterCallback("onMouseExit");
	mSprites[2]->unregisterCallback("onMouseExit");
	mSprites[3]->unregisterCallback("onMouseExit");
}

void SkillHoverComponent::registerSprite(Odyssey::Sprite2D* sprite)
{
	mSprites.push_back(sprite);
}

void SkillHoverComponent::skill1Enter()
{
	// Create skill hover hud if the pause menu is not active
	if (!GameUIManager::getInstance().GetPauseMenu(0)->getComponent<Odyssey::UICanvas>()->isActive())
	{
		// Create the skill popup for skill 1
		CreateSkillHoverPrefab(SkillNumber::Skill1);
	}
}

void SkillHoverComponent::skill2Enter()
{
	// Create skill hover hud if the pause menu is not active
	if (!GameUIManager::getInstance().GetPauseMenu(0)->getComponent<Odyssey::UICanvas>()->isActive())
	{
		// Create the skill popup for skill 2
		CreateSkillHoverPrefab(SkillNumber::Skill2);
	}
}

void SkillHoverComponent::skill3Enter()
{
	// Create skill hover hud if the pause menu is not active
	if (!GameUIManager::getInstance().GetPauseMenu(0)->getComponent<Odyssey::UICanvas>()->isActive())
	{
		// Create the skill popup for skill 3
		CreateSkillHoverPrefab(SkillNumber::Skill3);
	}
}

void SkillHoverComponent::skill4Enter()
{
	// Create skill hover hud if the pause menu is not active
	if (!GameUIManager::getInstance().GetPauseMenu(0)->getComponent<Odyssey::UICanvas>()->isActive())
	{
		// Create the skill popup for skill 4
		CreateSkillHoverPrefab(SkillNumber::Skill4);
	}
}

void SkillHoverComponent::skill1Exit()
{
	// Check if the skill is currently a lower opactiy meaning you don't have enough mana
	//if (mSprites[0]->getOpacity() == 1.0f)
	//{
		DestorySkillHoverPopup(SkillNumber::Skill1);
	//}
}

void SkillHoverComponent::skill2Exit()
{
	// Check if the skill is currently a lower opactiy meaning you don't have enough mana
	//if (mSprites[1]->getOpacity() == 1.0f)
	//{
		DestorySkillHoverPopup(SkillNumber::Skill2);
	//}
}

void SkillHoverComponent::skill3Exit()
{
	// Check if the skill is currently a lower opactiy meaning you don't have enough mana
	//if (mSprites[2]->getOpacity() == 1.0f)
	//{
		DestorySkillHoverPopup(SkillNumber::Skill3);
	//}
}

void SkillHoverComponent::skill4Exit()
{
	// Check if the skill is currently a lower opactiy meaning you don't have enough mana
	//if (mSprites[3]->getOpacity() == 1.0f)
	//{
		DestorySkillHoverPopup(SkillNumber::Skill4);
	//}
}

CharacterFactory::SkillHoverID SkillHoverComponent::GetSkillPopupPos()
{
	switch (mHudPositionEnum)
	{
		case HudPosition::Left:
			return CharacterFactory::SkillHoverID::LeftHUD;
			break;
		case HudPosition::Middle:
			return CharacterFactory::SkillHoverID::MiddleHUD;
			break;
		case HudPosition::Right:
			return CharacterFactory::SkillHoverID::RightHUD;
			break;
		default:
			return CharacterFactory::SkillHoverID::LeftHUD;
			break;
	}
}

void SkillHoverComponent::CreateSkillHoverPrefab(SkillNumber _skillNumber)
{
	// Enum to position the skill popup in the right spot
	CharacterFactory::SkillHoverID skillPopupPosition = GetSkillPopupPos();

	// Create the skill popup 
	Odyssey::Entity* prefab = CharacterFactory::getInstance().GetSkillHoverHUDPrefab(skillPopupPosition);
	DirectX::XMVECTOR vec = { 0.0f, 0.0f, 0.0f, 0.0f };
	Odyssey::EventManager::getInstance().publish(new Odyssey::SpawnEntityEvent(prefab, &mCurrentSkillPopups[_skillNumber], vec, vec));

	// Get the skill hud
	SkillHUDElements* skillHud = mCurrentSkillPopups[_skillNumber]->getComponent<SkillHUDElements>();

	// Assign the correct elements
	skillHud->ChangeSkillImage(mCharacterSkills[_skillNumber]->GetSkillIconPath());
	skillHud->ChangeSkillName(mCharacterSkills[_skillNumber]->GetSkillName());
	float manaCost = mCharacterSkills[_skillNumber]->GetManaCost();
	if (manaCost < 0.0f)
	{
		manaCost = 0.0f;
	}
	skillHud->ChangeManaCost(std::to_wstring((int)manaCost));
	skillHud->ChangeSkillDescription(mCharacterSkills[_skillNumber]->GetSkillDescription());
	skillHud->ChangeThemeColor(characterComponent->GetThemeColor()); // Set the color to the character's theme color
}

void SkillHoverComponent::DestorySkillHoverPopup(SkillNumber _skillNumber)
{
	// Destory the skill hover popup
	Odyssey::EventManager::getInstance().publish(new Odyssey::DestroyEntityEvent(mCurrentSkillPopups[_skillNumber]));
}