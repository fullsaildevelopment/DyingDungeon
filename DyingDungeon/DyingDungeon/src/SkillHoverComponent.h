#pragma once
#include "OdysseyEngine.h"
#include "Skills.h"
#include "CharacterFactory.h"

class SkillHoverComponent : public Odyssey::Component
{
	CLASS_DECLARATION(SkillHoverComponent)

	enum HudPosition
	{
		Left,
		Middle,
		Right,
	};

	enum SkillNumber
	{
		Skill1,
		Skill2,
		Skill3,
		Skill4,
	};

public:
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void initialize();
	virtual void onDestroy();
	void registerSprite(Odyssey::Sprite2D* sprite);
	void skill1Enter();
	void skill2Enter();
	void skill3Enter();
	void skill4Enter();
	void skill1Exit();
	void skill2Exit();
	void skill3Exit();
	void skill4Exit();
public:
	// Hold the Hud's position
	HudPosition mHudPositionEnum;
	
	// Hold the character's skill list
	std::vector<std::shared_ptr<Skills>> mCharacterSkills;

	// Hold the character component
	Character* characterComponent;

	// Get the skill sprites
	std::vector<Odyssey::Sprite2D*> GetSkillSprites() { return mSprites; }

private:
	// Hold the current prefab object
	Odyssey::Entity* mCurrentSkillPopups[4];

	// Hold a list of skill hover popups that need to be destroyed
	std::vector<Odyssey::Entity*> mSkillPopupsToDestroy;

	// List of the sprites which will be the skill icons
	std::vector<Odyssey::Sprite2D*> mSprites;

private: // Functions
	// Get the position that the skill popup will need to be at
	CharacterFactory::SkillHoverID GetSkillPopupPos();

	// Create the skill hover popup
	void CreateSkillHoverPrefab(SkillNumber _skillNumber);

	// Destory the skill hover popup
	void DestorySkillHoverPopup(SkillNumber _skillNumber);
};