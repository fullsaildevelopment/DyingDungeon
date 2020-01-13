#pragma once
#include "Component.h"
#include "Animator.h"
#include "Rectangle2D.h"
#include "Text2D.h"
#include "Skills.h"
#include <vector>

#define TOTALSKILLS 7

enum class HEROID { Paladin = 0 };
enum class ENEMYID { Skeleton = 0 };
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
	// Dead Status Functions
	bool IsDead();
	void SetDead(bool deadStatus);
	// EXP Functions
	void AddExp(float exp);
	float GetExp();
	// IsHero Functions
	bool IsHero();
	void SetHero(bool heroStat);
	// Name Functions
	std::string GetName();
	// Skills Functions
	void SetName(std::string newName);
	std::vector<std::shared_ptr<Skills>> GetSkills();
	// mProvoked Functions
	Character* GetProvoked();
	void SetProvoked(Character* provoker);
	// State functions
	STATE GetState();
	void SetState(STATE newState);
	// Impact Indicator Setter
	void SetImpactIndicator(std::shared_ptr<Odyssey::Entity> _impactIndicatorModel) { mImpactIndicator = _impactIndicatorModel; }
	/////End of Get and Set Functions/////
	// Status Effect Functions
	void AddStatusEffect(std::shared_ptr<StatusEffect> newEffect);
	void ManageStatusEffects(std::vector<std::shared_ptr<StatusEffect>>& effectList);
	bool ManageAllEffects();
	void ClearStatusEffects();
	//Update HealthBar UI
	void UpdateHealthBar();
	//Update ManaBar UI
	void UpdateManaBar();
	
	Odyssey::Rectangle2D* pHealthBar;
	Odyssey::Rectangle2D* pManaBar;
	Odyssey::Text2D* pTurnNumber;

	// TODO: FOR BUILD ONLY FIX LATER
	Odyssey::Text2D* pDmgText;
	bool mDisplaying;

protected:
	//Stats
	bool mHero;
	bool mDead;
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
	std::shared_ptr<Odyssey::Entity> mImpactIndicator;
private:
	float mPrevHealth;
	float mPrevMana;
};

