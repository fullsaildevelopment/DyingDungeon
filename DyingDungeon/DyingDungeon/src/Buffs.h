#pragma once
#include "Skills.h"
#include <memory>

class Buffs : public Skills
{
public:
	
private:
public:
	// Constructors
	Buffs() = default;
	Buffs(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff);
	Buffs(std::wstring skillName, std::string animationId, float animationTiming, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff, bool isAOE);
	// Use the skill 
	void Use(Character& caster, Character& target);
private:
};

