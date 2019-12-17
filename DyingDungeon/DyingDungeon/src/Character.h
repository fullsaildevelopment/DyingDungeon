#pragma once
#include "Component.h"
#include "Skills.h"
#include <vector>

#define TOTALSKILLS 5

enum class HEROID { Paladin = 0 };
enum class ENEMYID { Skeleton = 0 };

class Character : public Odyssey::Component
{
	CLASS_DECLARATION(Character)

public:
	Character() = default;
public:
	virtual void initialize();
	virtual void update(double deltaTime);

	//Attack Functions
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> playerTeam, std::vector<std::shared_ptr<Odyssey::GameObject>> enemyTeam);
	virtual void Die();

	void TakeDamage(float dmg);
	void DepleteMana(float manaCost);

	/////Get and Set Functions/////
	// HP Functions
	float GetHP();
	void SetHP(float m_HP);
	float GetMaxHP();
	// Mana Functions
	float GetMana();
	void SetMana(float Mana);
	// Shield Functions
	float GetShielding();
	void AddShielding(float shield);
	// Attack Functions
	float GetAtk();
	void IncreaseAtk(float statIncrease);
	void DecreaseAtk(float statDecrease);
	// Defense Functions
	float GetDef();
	void IncreaseDef(float statIncrease);
	void DecreaseDef(float statDecrease);
	// Speed Functions
	float GetSpeed();
	void IncreaseSpd(float statIncrease);
	void DecreaseSpd(float statDecrease);
	// Dead Status Functions
	bool IsDead();
	void SetDead(bool deadStatus);
	// EXP Functions
	void AddExp(float exp);
	// IsHero Functions
	bool IsHero();
	void SetHero(bool heroStat);
	// Name Functions
	std::string GetName();
	void SetName(std::string newName);
	// Skill Functions
	void SetSkills(Skills newSkillList);
	Skills* GetSkills();
	// Status Effect Functions
	void AddStatusEffect(Buffs* newEffect);
	void ManageStatusEffects();
	/////End of Get and Set Functions/////
protected:
	bool mHero;
	bool mDead;
	//Stats
	float mCurrentHP;
	float mCurrentMana;
	float mBaseMaxHP;
	float mBaseMaxMana;
	float mShielding;
	float mAttack;
	float mDefense;
	float mSpeed;
	float mEXP;
	std::string mName;
	Skills mSkillList[TOTALSKILLS];
	std::vector<Buffs*> mStatusEffects;
private:

};

