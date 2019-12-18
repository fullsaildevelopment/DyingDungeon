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
	// If attack or support
	bool mAttack;
	// Name of skill
	std::string mName;
	// Animation ID
	std::string mAnimationId;
//public and private functions
public:
	Skills() = default;
	//constructor, give how much damage you want the skill to do and how much mana you want it to cost 
	Skills(float dps, float mana,bool attack, std::string skillName, std::string animationID);
	//additionally if it has a buff/debuff give it here 
	Skills(float dps, float mana, bool attack, Buffs buff, std::string skillName, std::string animationID);
	//deconstructor
	~Skills();
	//get how much mana the skill cost
	float GetManaCost();
	// Get if it has a buff or not
	Buffs GetBuff();
	// Get Skill name
	std::string GetName();
	//Get Damage
	float GetDamage();
	// Get Attack
	bool IsAttack();
	// Gets the Animation ID
	std::string GetAnimationId();
	//use the skill
	void Use(Character& caster, Character& target);
private:

};

