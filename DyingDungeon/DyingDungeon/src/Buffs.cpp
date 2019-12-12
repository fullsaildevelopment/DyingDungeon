#include "Buffs.h"
#include "Character.h"
#include "GameObject.h"

CLASS_DEFINITION(Component, Buffs)

Buffs::Buffs(float* stat, float effect, int duration)
{
	m_statToAffect = stat;
	m_amountOfEffect = effect;
	m_duration = duration;
}
Buffs::~Buffs()
{
	*m_statToAffect -= m_amountOfEffect;
}
void Buffs::initialize(Odyssey::GameObject* parent)
{
	*m_statToAffect += m_amountOfEffect;
}