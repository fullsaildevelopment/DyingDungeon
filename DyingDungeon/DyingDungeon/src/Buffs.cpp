#include "Buffs.h"

CLASS_DEFINITION(Component, Buffs)

Buffs::Buffs(float* stat, float effect, int duration)
{
	m_statToAffect = stat;
	m_amountOfEffect = effect;
	m_duration = duration;
}
Buffs::~Buffs()
{

}
void Buffs::initialize(Odyssey::GameObject* parent)
{

}