#pragma once
// enum used for readablity 
enum class STATS { NONE = 0, HP, MP, Atk, Def, Spd, Shd, Stn };
class Character;
class StatusEffect
{
	// public and private variables
public:
protected:
	float mAmountOfEffect;
	int mDuration;
	Character* mRecipient;
private:
	// public and private functions
public:
	// Constructors, Deconstructors
	StatusEffect() = default;
	~StatusEffect() = default;
	// Application functions
	virtual void Apply() = 0;
	virtual void Remove() = 0;
	virtual void Use() = 0;
	//  mDuration functions
	int GetDuration();
	void SetDuration(int newDuration);
	void ReduceDuration(int deduction);
	// mAmountOfEffect functions
	float GetAmountOfEffect();
	// mRecipient functions
	Character* GetRecipient();
	void SetRecipient(Character * recipient);
};

