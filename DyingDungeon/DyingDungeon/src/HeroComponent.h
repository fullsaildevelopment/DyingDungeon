#pragma once
#include "Character.h"
#include "InputManager.h"

// Tristance dumbass typeDef
typedef std::vector<std::shared_ptr<Odyssey::Entity>> EntityList;

class HeroComponent : public Character
{
	CLASS_DECLARATION(HeroComponent)

public:
	// Constructors
	HeroComponent() = default;

	// Pass in Hero identifcation to get a specific Hero class	
	HeroComponent(GameplayTypes::HEROID id);

	// Destructor
	~HeroComponent();

	// Function that allows the player to take thier turn, Character Controler
	virtual bool TakeTurn(EntityList heros, EntityList enemies);

	// Function that gets called to set the character state to dead, along with all other necessary variables
	virtual void Die();

	// Returns the characters skill list
	virtual std::vector<std::shared_ptr<Skills>> GetSkills();

	// Get the hero id
	GameplayTypes::HEROID GetID();

private:
	// Pointer to the current skill the player has selected
	Skills* mCurrentSkill;

	// Pointer to the current target the player has selected
	Character* mCurrentTarget;

	// Heros Id
	GameplayTypes::HEROID mID;

	// Function that gets called to manage the state in which the player is selecting a skill to use
	void SelctionState(int moveIndex);

	// Function that gets called to manage the state in which the player is selecting a target to use its skill on 
	bool SelectTarget(EntityList targets, int& targetIndex);

	// Function that gets called to send the player back to the selection state
	void ResetToSelection();

	// Function that sends the state into the inprogress state, queing animations, and setting varia bles for particle effect locations
	void BeginAttack(EntityList targets);

public:
	virtual void initialize();
private:
	// List that contains the list ofheros and enemies
	EntityList mHeroList;
	EntityList mEnemyList;
	// Variable to toggle when im cheating
	bool mIsCheating;

	//Set up for clickable UI
	void SetupClickableUI();

	// Skill callbacks
	void Skill1Callback();
	void Skill2Callback();
	void Skill3Callback();
	void Skill4Callback();
};

