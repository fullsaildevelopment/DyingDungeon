#pragma once

class Skills
{
//public and private variables
public:
	
private:
	//how much damage this skill will do to HP
	float m_damage;
	//how much mana it will cost the caster
	float m_mpCost;
public:
	Skills(float dps, float mana);
	void Use();
private:

};

