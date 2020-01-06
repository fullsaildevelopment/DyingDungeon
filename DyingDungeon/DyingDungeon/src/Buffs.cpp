#include "Buffs.h"
#include "Character.h"

Buffs::Buffs(std::string skillName, std::string animationId, float mpCost, StatusEffect* buff)
{
	mTypeId = SKILLTYPE::BUFF;
	mName = skillName;
	mAnimationId = animationId;
	mMpCost = mpCost;
	mBuff = buff;
}
void Buffs::Use(Character& caster, Character& target)
{
	if (mBuff != nullptr)
	{
		caster.DepleteMana(mMpCost);
		mBuff->Apply(target);
		std::cout << caster.GetName() << " used " << mName << " on " << target.GetName() << "." << std::endl;
	}
}
StatusEffect* Buffs::GetStatusEffect()
{
	return mBuff;
 }
