#include "Heal.h"
#include "Character.h"
// Use the heal on the target
void Heal::Use(Character& caster, Character& target)
{
	caster.DepleteMana(mMpCost);
	target.ReceiveHealing(mHealing);
}
// Get the amount the heal is for
float Heal::GetHealing()
{
	return mHealing;
}
