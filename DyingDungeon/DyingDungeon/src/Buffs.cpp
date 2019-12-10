#include "Buffs.h"

Buffs::Buffs(float* stat, float effect, int duration)
{
	m_statToAffect = stat;
	m_amountOfEffect = effect;
	duration = duration;
}
Buffs::~Buffs()
{

}
void initialize(Odyssey::GameObject* parent, float* stat, float effect, int duration)
{
	
}