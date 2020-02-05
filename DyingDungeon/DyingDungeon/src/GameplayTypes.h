#pragma once

class GameplayTypes
{
public:
	// Hero ID values
	enum class HEROID { NONE = -1, Paladin = 0, Mage, Bard, Warrior, Rogue };

	// Enemy ID values
	enum class ENEMYID { NONE = -1, Skeleton = 0, Ganfaul, MeleeAdd, Caster };

	// Skill types
	enum class SKILLTYPE { NONE = -1, ATTACK = 0, HEAL, BUFF, DEBUFF };

};