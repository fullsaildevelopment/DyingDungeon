#include "Buffs.h"
#include "Character.h"

Buffs::Buffs(std::string skillName, std::string animationId, float mpCost, std::shared_ptr<StatusEffect> buff, bool isBuff, bool isAOE)
{
	if (isBuff)
		mTypeId = SKILLTYPE::BUFF;
	else
		mTypeId = SKILLTYPE::DEBUFF;
	mName = skillName;
	mAnimationId = animationId;
	mMpCost = mpCost;
	mBuff = buff;
	mIsAOE = isAOE;
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
	return mBuff.get();
 }
