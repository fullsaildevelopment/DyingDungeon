#pragma once
#include "Buffs.h"

//forward declare charater class
class Character;

class Skills
{
//public and private variables
public:
private:
	//how much damage this skill will do to HP
	float mDamage;
	//how much mana it will cost the caster
	float mMpCost;
	// If any the buff it contains
	Buffs mBuff;
	// Name of skill
	std::string mName;
//public and private functions
public:
	Skills() = default;
	//constructor, give how much damage you want the skill to do and how much mana you want it to cost 
	Skills(float dps, float mana, std::string skillName);
	//additionally if it has a buff/debuff give it here 
	Skills(float dps, float mana, Buffs buff, std::string skillName);
	//deconstructor
	~Skills();
	//get how much mana the skill cost
	float GetManaCost();
	// Get if it has a buff or not
	Buffs GetBuff();
	// Get Skill name
	std::string GetName();
	//use the skill
	void Use(Character& caster, Character& target);
private:

};

