#include "Skills.h"
#include "EnemyComponent.h"

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
void Skills::Use(EnemyComponent& target)
{
	target.SetHP(target.GetHP() - m_damage);
}
