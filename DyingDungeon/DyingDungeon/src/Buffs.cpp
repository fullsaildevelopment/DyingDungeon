#include "Buffs.h"
#include "Character.h"

// Constructors and destructors
Buffs::Buffs(STATS effectedStat, float effect, int duration, bool isBleed, Character* recipient)
{
	mAmountOfEffect = effect;
	mDuration = duration;
	mEffectedStat = effectedStat;
	mBleed = isBleed;
	mRecipient = recipient;
}
Buffs::~Buffs()
{
	mRecipient = nullptr;
}

// add the Inital Effect to the target, called once it is applied
void Buffs::InitalEffect()
{
	switch (mEffectedStat)
	{
	case STATS::HP:
	{
		break;
	}
	case STATS::MP:
	{
		break;
	}
	case STATS::Atk:
	{
		std::cout << "Attack Buff on " << mRecipient->GetName() << " Gained " << mAmountOfEffect << "%" << std::endl;
		mRecipient->IncreaseAtk(mAmountOfEffect);
		break;
	}
	case STATS::Def:
	{
		std::cout << "Defense Buff on " << mRecipient->GetName() << " Gained " << mAmountOfEffect << "%" << std::endl;
		mRecipient->IncreaseDef(mAmountOfEffect);
		break;
	}
	case STATS::Spd:
	{
		std::cout << "Speed Buff on " << mRecipient->GetName() << " Gained " << mAmountOfEffect << "%" << std::endl;
		mRecipient->IncreaseSpd(mAmountOfEffect);
		break;
	}
	case STATS::Shd:
	{
		std::cout << "Sheild on " << mRecipient->GetName() << " for " << mAmountOfEffect << "Temp HP" << std::endl;
		mRecipient->AddShielding(mAmountOfEffect);
		break;
	}
	default:
		break;
	}
}

// Revert the effected stats back to how they were before the buff, called once duration == 0 && !IsBleed()
void Buffs::RevertEffect()
{
	switch (mEffectedStat)
	{
	case STATS::HP:
	{
		break;
	}
	case STATS::MP:
	{
		break;
	}
	case STATS::Atk:
	{
		std::cout << "Attack Buff off " << mRecipient->GetName() << " Lost " << mAmountOfEffect << "%" << std::endl;
		mRecipient->DecreaseAtk(mAmountOfEffect);
		break;
	}
	case STATS::Def:
	{
		std::cout << "Defense Buff off " << mRecipient->GetName() << " Lost " << mAmountOfEffect << "%" << std::endl;
		mRecipient->DecreaseDef(mAmountOfEffect);
		break;
	}
	case STATS::Spd:
	{
		std::cout << "Speed Buff off " << mRecipient->GetName() << " Lost " << mAmountOfEffect << "%" << std::endl;
		mRecipient->DecreaseSpd(mAmountOfEffect);
		break;
	}
	case STATS::Shd:
	{
		std::cout << "Sheild fell off " << mRecipient->GetName() << std::endl;
		//remove sheilding
		break;
	}
	default:
		break;
	}
}

// Effect that takes effect after the charaters turn ends
void Buffs::Bleed()
{
	switch (mEffectedStat)
	{
	case STATS::HP:
	{
		mRecipient->TakeDamage(mAmountOfEffect * mRecipient->GetMaxHP());
		if(mAmountOfEffect > 0)
			std::cout << mRecipient->GetName() << " took " << (mAmountOfEffect * mRecipient->GetMaxHP()) - ((mAmountOfEffect * mRecipient->GetMaxHP()) * mRecipient->GetDef()) << " damage in bleeding "  << mRecipient->GetName() << " now has " << mRecipient->GetHP() << " HP\n" << std::endl;
		else
			std::cout << mRecipient->GetName() << " received " << (mAmountOfEffect * mRecipient->GetMaxHP()) << " HP in healing " << mRecipient->GetName() << " now has " << mRecipient->GetHP() << " HP\n" << std::endl;
		if (mRecipient->GetHP() <= 0)
			mRecipient->Die();
		break;
	}
	case STATS::MP:
	{
		break;
	}
	case STATS::Atk:
	{
		break;
	}
	case STATS::Def:
	{
		break;
	}
	case STATS::Shd:
	{
		break;
	}
	default:
		break;
	}
}

// Get how many turns are left for this buff
int Buffs::GetDuration()
{
	return mDuration;
}

// Set how many turns are left this buff
void Buffs::SetDuration(int newDuration)
{
	mDuration = newDuration;
}

// Reduce the duration of the buff by the passed in value
void Buffs::ReduceDuration(int deduction)
{
	mDuration -= deduction;
}

// Check if the buff is a bleed or not
bool Buffs::IsBleed()
{
	return mBleed;
}

// Get what stat is effected by this buff
STATS Buffs::GetEffectedStat()
{
	return mEffectedStat;
}

// Set which stat is effected by this buff
void Buffs::SetEffectedStat(STATS statEffected)
{
	mEffectedStat = statEffected;
}

// Get how much the stat will be effected in % 
float Buffs::GetAmountOfEffect()
{
	return mAmountOfEffect;
}

// Get the target of the buff
Character* Buffs::GetRecipient()
{
	return mRecipient;
}

// Set the target of the buff
void Buffs::SetRecipient(Character* recipient)
{
	mRecipient = recipient;
}
