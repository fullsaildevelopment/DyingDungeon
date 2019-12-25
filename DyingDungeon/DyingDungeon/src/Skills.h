#pragma once
#include "StatusEffect.h"
enum class SKILLTYPE {ATTACK = 0, HEAL, BUFF, DEBUFF};
//forward declare charater class
class Character;

class Skills
{
//public and private variables
public:
protected:
	SKILLTYPE mTypeId;
	//how much mana it will cost the caster
	float mMpCost;
	// Name of skill
	std::string mName;
	// Animation ID
	std::string mAnimationId;
private:
//public and private functions
public:
	//constructor
	Skills() = default;
	//deconstructor
	~Skills() = default;
	//get how much mana the skill cost
	float GetManaCost();
	// Get Skill name
	std::string GetName();
	// Gets the Animation ID
	std::string GetAnimationId();
	// Get TypeID
	SKILLTYPE GetTypeId();
	//use the skill
	virtual void Use(Character& caster, Character& target) = 0;
private:

};

