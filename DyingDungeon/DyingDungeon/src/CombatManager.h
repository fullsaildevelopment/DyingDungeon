#pragma once
class CombatManager
{
public:

	enum class CharacterToAttack
	{
		NoPlayer,
		Hero1,
		Hero2,
		Hero3,
		Enemy1,
		Enemy2,
		Enemy3,
	};

public: // Singleton pattern
	/**
	 *	Get the singleton instance of the game ui manager.
	 *	@param[in] void
	 *	@return InputManager& The singleton instance of the game ui manager.
	 */
	static CombatManager& getInstance();
	~CombatManager() { }

private: // Singleton pattern
	CombatManager() {}

public: // Functions

	// Getters
	CharacterToAttack GetCharacterToAttack() { return mCharacterToAttack; }

	// Setters
	void SetCharacterToAttack(CharacterToAttack _enum) { mCharacterToAttack = _enum; }

private: // variables

	// Save the player to attack
	CharacterToAttack mCharacterToAttack;

};

