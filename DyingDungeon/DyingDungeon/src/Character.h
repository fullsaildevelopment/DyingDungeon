#pragma once
#include "Component.h"
#include "Animator.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Skills.h"
#include "GameUIManager.h"
#include <vector>

enum class HEROID { Paladin = 0, Mage, Bard };
enum class ENEMYID { Skeleton = 0, Ganfaul };
enum class STATE { NONE = 0, STUNNED, SELECTMOVE, SELECTTARGET, CONFIRM, INPROGRESS, FINISHED, DEAD };

class Character : public Odyssey::Component
{
	CLASS_DECLARATION(Character)

public:
	Character();
public:
	virtual void initialize();
	virtual void update(double deltaTime);

	//Attack Functions
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::Entity>> playerTeam, std::vector<std::shared_ptr<Odyssey::Entity>> enemyTeam);
	virtual void Die();

	//Skills
	virtual std::vector<std::shared_ptr<Skills>> GetSkills();

	void TakeDamage(float dmg);
	void ReceiveHealing(float healing);
	void DepleteMana(float manaCost);

	/////Get and Set Functions/////
	// HP Functions
	float GetHP();
	void SetHP(float m_HP);
	float GetMaxHP();
	// Mana Functions
	float GetMana();
	float GetMaxMana();
	void SetMana(float Mana);
	// Shield Functions
	float GetShielding();
	void SetShielding(float shield);
	// Attack Functions
	float GetAtk();
	void IncreaseAtk(float statIncrease);
	void DecreaseAtk(float statDecrease);
	// Defense Functions
	float GetDef();
	float GetBaseDef();
	void IncreaseDef(float statIncrease);
	void DecreaseDef(float statDecrease);
	// Speed Functions
	float GetSpeed();
	float GetBaseSpeed();
	void IncreaseSpd(float statIncrease);
	void DecreaseSpd(float statDecrease);
	// EXP Functions
	void AddExp(float exp);
	float GetExp();
	// IsHero Functions
	bool IsHero();
	void SetHero(bool heroStat);
	// Name Functions
	std::string GetName();
	void SetName(std::string newName);
	// mProvoked Functions
	Character* GetProvoked();
	void SetProvoked(Character* provoker);
	// State functions
	STATE GetState();
	void SetState(STATE newState);
	// Impact Indicator Setter
	void SetImpactIndicator(std::shared_ptr<Odyssey::Entity> _impactIndicatorModel) { mImpactIndicator = _impactIndicatorModel; }
	// Impact Indicator Getter
	std::shared_ptr<Odyssey::Entity> GetInpactIndicator() { return mImpactIndicator; }
	/////End of Get and Set Functions/////
	// Status Effect Functions
	bool AddStatusEffect(std::shared_ptr<StatusEffect> newEffect);
	void ManageStatusEffects(std::vector<std::shared_ptr<StatusEffect>>& effectList);
	bool ManageAllEffects();
	void ClearStatusEffects();
	//Update HealthBar UI
	void UpdateHealthBar();
	//Update ManaBar UI
	void UpdateManaBar();

	std::wstring FormatToPercentageW(float number);

	// Blood particle effect functions
	void SetPSBlood(Odyssey::ParticleSystem* newBloodEffect);
	Odyssey::ParticleSystem* GetPSBlood();
	
	Odyssey::Rectangle2D* pHealthBar;
	Odyssey::Rectangle2D* pManaBar;
	Odyssey::Text2D* pTurnNumber;
	Odyssey::Text2D* mBigHpText;
	Odyssey::Text2D* mHpText;
	Odyssey::Text2D* mMpText;

	// TODO: FOR BUILD ONLY FIX LATER
	Odyssey::Text2D* pDmgText;
	bool mDisplaying;

protected:
	//Stats
	bool mHero;
	float mCurrentHP;
	float mCurrentMana;
	float mBaseMaxHP;
	float mBaseMaxMana;
	float mBaseDefense;
	float mBaseSpeed;
	float mShielding;
	float mAttack;
	float mDefense;
	float mSpeed;
	float mEXP;
	Character* mProvoked;
	std::string mName;
	std::vector<std::shared_ptr<Skills>> mSkillList;
	std::vector<std::shared_ptr<StatusEffect>> mDebuffs;
	std::vector<std::shared_ptr<StatusEffect>> mBuffs;
	std::vector<std::shared_ptr<StatusEffect>> mBleeds;
	std::vector<std::shared_ptr<StatusEffect>> mRegens; 
	std::vector<std::shared_ptr<StatusEffect>> mSheilds;
	Odyssey::Animator* mAnimator;
	STATE mCurrentState;
	Odyssey::ParticleSystem* mBloodParticleEffect;
	public:
	std::shared_ptr<Odyssey::Entity> mImpactIndicator;
private:
	float mPrevHealth;
	float mPrevMana;
};

