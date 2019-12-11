#include "Skills.h"
#include "Character.h"

Skills::Skills(float dps, float mana)
{
	m_damage = dps;
	m_mpCost = mana;
}
Skills::~Skills()
{
}
float Skills::GetManaCost()
{
	return m_mpCost;
}
void Skills::Use(Character& caster,Character& target)
{
	caster.SetMana(caster.GetMana() - m_mpCost);
	target.SetHP(target.GetHP() - m_damage);

	std::cout << caster.name << " attacked " << target.name << " for " << m_damage << std::endl;
	std::cout << target.name << " now has " << target.GetHP() << "HP\n" << std::endl;

}
