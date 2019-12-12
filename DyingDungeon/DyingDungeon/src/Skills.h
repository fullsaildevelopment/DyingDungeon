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
//public and private functions
public:
	Skills() = default;
	//constructor, give how much damage you want the skill to do and how much mana you want it to cost 
	Skills(float dps, float mana);
	//deconstructor
	~Skills();
	//get how much mana the skill cost
	float GetManaCost();
	//use the skill
	void Use(Character& caster, Character& target);
private:

};

