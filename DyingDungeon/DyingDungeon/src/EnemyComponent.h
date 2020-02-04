#pragma once
#include "Character.h"
#include "AiMoves.h"
#include "GameplayTypes.h"

class EnemyComponent : public Character
{
	CLASS_DECLARATION(EnemyComponent)

public:
	// Constructors
	EnemyComponent() = default;

	// Pass in Enemy identifcation to get a specific Enemy class	
	EnemyComponent(GameplayTypes::ENEMYID _enemyID);

	// Destructors
	~EnemyComponent();

	// Function that allows the AI to take thier turn, Character Controler
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);

	// Function that gets called to set the character state to dead, along with all other necessary variables
	virtual void Die();

	// Function that sends the state into the inprogress state, queing animations, and setting varia bles for particle effect locations
	void BeginAttack(std::vector<std::shared_ptr<Odyssey::Entity>> targets);

	// Returns the characters skill list
	virtual std::vector<std::shared_ptr<Skills>> GetSkills();

	// Function pointer that will point to mechanics for enemys 
	void (EnemyComponent::*mMechPtr)() = nullptr;

	GameplayTypes::ENEMYID GetID();

	// Ganfouls game mechanic function
	void GanfaulPhaseMechanic();

private:
	// Idk Bryce what does this do? (Fill out this line for comments)
	int currentSkillMoveCheck = 0;

	// Enemy ID
	GameplayTypes::ENEMYID mID;

	// Contains the move thae AI will make
	AIMoves mMoves;

	// Used to detrim what kind of move the AI should make
	GameplayTypes::SKILLTYPE mMoveOverride;
};

