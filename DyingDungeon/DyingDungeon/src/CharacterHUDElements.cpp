#include "CharacterHUDElements.h"

CLASS_DEFINITION(Component, CharacterHUDElements)

std::shared_ptr<Odyssey::Component> CharacterHUDElements::clone() const
{
	return std::make_shared<CharacterHUDElements>(*this);
}

void CharacterHUDElements::initialize()
{
}

void CharacterHUDElements::update(double deltaTime)
{
	for (int i = 0; i < mSkillBackgroundList.size(); i++)
	{
		if (mSkillBackgroundList[i] != nullptr)
		{
			// Save the opacity
			float newOpacity = 0.0f;

			//Check if we need to fade away or com back to full opacity
			if (mFadingDown[i])
				newOpacity = mSkillBackgroundList[i]->getOpacity() + (mDeltaOpacity * -deltaTime);
			else
				newOpacity = mSkillBackgroundList[i]->getOpacity() + (mDeltaOpacity * deltaTime);

			// Set opacity
			mSkillBackgroundList[i]->setOpacity(newOpacity);

			// Check to see if we reached 0.0f or 1.0f to reverse the effect
			if (mSkillBackgroundList[i]->getOpacity() <= 0.0f || mSkillBackgroundList[i]->getOpacity() >= 1.0f)
			{
				mFadingDown[i] != mFadingDown[i];
			}
		}
	}
}

void CharacterHUDElements::onDestroy()
{
}

void CharacterHUDElements::ChangeSkill(Skills* _skill, int _numOfSkill)
{
	switch (_numOfSkill)
	{
		case 1:
			// Set the new sprite image
			mSkill1->setSprite(_skill->GetSkillIconPath(), (UINT)mSkill1->getDimensions().x, (UINT)mSkill1->getDimensions().y);
			break;
		case 2:
			// Set the new sprite image
			mSkill2->setSprite(_skill->GetSkillIconPath(), (UINT)mSkill2->getDimensions().x, (UINT)mSkill2->getDimensions().y);
			break;
		case 3:
			// Set the new sprite image
			mSkill3->setSprite(_skill->GetSkillIconPath(), (UINT)mSkill3->getDimensions().x, (UINT)mSkill3->getDimensions().y);
			break;
		case 4:
			// Set the new sprite image
			mSkill4->setSprite(_skill->GetSkillIconPath(), (UINT)mSkill4->getDimensions().x, (UINT)mSkill4->getDimensions().y);
			break;
		default:
			break;
	}
}

void CharacterHUDElements::ChangeSkillBackgroundColors(DirectX::XMFLOAT3 _newColor)
{
	for (int i = 0; i < mSkillBackgroundList.size(); i++)
	{
		mSkillBackgroundList[i]->setColor(_newColor);
	}
}

void CharacterHUDElements::ClearStatusEffects()
{
	mAttackUp->setVisible(false);
	mAttackDown->setVisible(false);
	mDefenseUp->setVisible(false);
	mDefenseDown->setVisible(false);
	mSpeedUp->setVisible(false);
	mSpeedDown->setVisible(false);
	mBleedBuff->setVisible(false);
	mStunBuff->setVisible(false);
	mProvokeBuff->setVisible(false);
	mRegenBuff->setVisible(false);
	mShieldBuff->setVisible(false);
}
