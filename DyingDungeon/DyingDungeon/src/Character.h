#pragma once
#include "Component.h"
#include "Skills.h"
#include <vector>

class Character : public Odyssey::Component
{
	CLASS_DECLARATION(Character)

public:
	Character() = default;
public:
	virtual void initialize(Odyssey::GameObject* parent);
	virtual void update(double deltaTime);

	//Attack Functions
	virtual bool TakeTurn(std::vector<std::shared_ptr<Odyssey::GameObject>> _allCharacters);


	/////Get and Set Functions/////

	//HP Functions
	float GetHP();
	void SetHP(float m_HP);
	//Mana Functions
	float GetMana();
	void SetMana(float Mana);
	//Dead Status Functions
	bool IsDead();
	void SetDead(bool deadStatus);
	//isHero Functions
	bool IsHero();
	void SetHero(bool heroStat);
	// Name Fucntions
	std::string GetName();
	void SetName(std::string newName);
	/////End of Get and Set Functions/////


protected:
	bool mHero;
	bool mDead;
	float mHP;
	float mMana;
	Skills skillList[4];
	std::string mName;


private:

};
