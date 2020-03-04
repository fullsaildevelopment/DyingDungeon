#pragma once
#include "Component.h"
#include "UICanvas.h"
#include "Sprite2D.h"
#include "Text2D.h"
#include "Rectangle2D.h"
#include "Skills.h"

class CharacterHUDElements : public Odyssey::Component
{
	CLASS_DECLARATION(CharacterHUDElements)

public:
	CharacterHUDElements() = default;
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestroy();
public:
	// Setters

	// HUD Position
	void SetHudPosition(DirectX::XMFLOAT2 _hudPos) { mHudPosition = _hudPos; }

	// HUD Elements
	void SetCanvas(Odyssey::UICanvas* _canvas) { mCanvas = _canvas; }
	void SetPortrait(Odyssey::Sprite2D* _sprite) { mPortrait = _sprite; }
	void SetCharacterName(Odyssey::Text2D* _text) { mCharacterName = _text; }
	void SetXpBar(Odyssey::Rectangle2D* _rect) { mXpBar = _rect; }
	void SetLevelNumber(Odyssey::Text2D* _text) { mLevelNumber = _text; }
	void SetAttackNumber(Odyssey::Text2D* _text) { mAttackNumber = _text; }
	void SetDefenseNumber(Odyssey::Text2D* _text) { mDefenseNumber = _text; }
	void SetSpeedNumber(Odyssey::Text2D* _text) { mSpeedNumber = _text; }
	void SetHealthBar(Odyssey::Rectangle2D* _rect) { mHealthBar = _rect; }
	void SetHealthNumber(Odyssey::Text2D* _text) { mHealthNumber = _text; }
	void SetShieldBar(Odyssey::Rectangle2D* _rect) { mShieldBar = _rect; }
	void SetManaBar(Odyssey::Rectangle2D* _rect) { mManaBar = _rect; }
	void SetManaNumber(Odyssey::Text2D* _text) { mManaNumber = _text; }
	void SetTurnNumber(Odyssey::Text2D* _text) { mTurnNumber = _text; }
	void SetHudBlocker(Odyssey::Rectangle2D* _rect) { mHudBlocker = _rect; }

	// Status Effect and Buff Elements
	void SetAttackUpBuff(Odyssey::Sprite2D* _sprite) { mAttackUp = _sprite; }
	void SetAttackDownBuff(Odyssey::Sprite2D* _sprite) { mAttackDown = _sprite; }
	void SetDefenseUpBuff(Odyssey::Sprite2D* _sprite) { mDefenseUp = _sprite; }
	void SetDefenseDownBuff(Odyssey::Sprite2D* _sprite) { mDefenseDown = _sprite; }
	void SetSpeedUpBuff(Odyssey::Sprite2D* _sprite) { mSpeedUp = _sprite; }
	void SetSpeedDownBuff(Odyssey::Sprite2D* _sprite) { mSpeedDown = _sprite; }
	void SetBleedBuff(Odyssey::Sprite2D* _sprite) { mBleedBuff = _sprite; }
	void SetStunBuff(Odyssey::Sprite2D* _sprite) { mStunBuff = _sprite; }
	void SetProvokeBuff(Odyssey::Sprite2D* _sprite) { mProvokeBuff = _sprite; }
	void SetRegenBuff(Odyssey::Sprite2D* _sprite) { mRegenBuff = _sprite; }
	void SetShieldBuff(Odyssey::Sprite2D* _sprite) { mShieldBuff = _sprite; }

	// Skill Elements
	void SetSkill1(Odyssey::Sprite2D* _sprite) { mSkill1 = _sprite; }
	void SetSkill2(Odyssey::Sprite2D* _sprite) { mSkill2 = _sprite; }
	void SetSkill3(Odyssey::Sprite2D* _sprite) { mSkill3 = _sprite; }
	void SetSkill4(Odyssey::Sprite2D* _sprite) { mSkill4 = _sprite; }

	// Skill Backgrounds
	void SetSkillBackgrounds(Odyssey::Rectangle2D* _rect) { mSkillBackgroundList.push_back(_rect); }

	// Getters

	// HUD Elements
	Odyssey::UICanvas* GetCanvas() { return mCanvas; }
	Odyssey::Sprite2D* GetPortrait() { return mPortrait; }
	Odyssey::Rectangle2D* GetXpBar() { return mXpBar; }
	Odyssey::Text2D* GetCharacterName() { return mCharacterName; }
	Odyssey::Text2D* GetLevelNumber() { return mLevelNumber; }
	Odyssey::Text2D* GetAttackNumber() { return mAttackNumber; }
	Odyssey::Text2D* GetDefenseNumber() { return mDefenseNumber; }
	Odyssey::Text2D* GetSpeedNumber() { return mSpeedNumber; }
	Odyssey::Rectangle2D* GetHealthBar() { return mHealthBar; }
	Odyssey::Text2D* GetHealthNumber() { return mHealthNumber; }
	Odyssey::Rectangle2D* GetShieldBar() { return mShieldBar; }
	Odyssey::Rectangle2D* GetManaBar() { return mManaBar; }
	Odyssey::Text2D* GetManaNumber() { return mManaNumber; }
	Odyssey::Text2D* GetTurnNumber() { return mTurnNumber; }
	Odyssey::Rectangle2D* GetHudBlocker() { return mHudBlocker; }

	// Skills Elements
	Odyssey::Sprite2D* GetSkill1() { return mSkill1; }
	Odyssey::Sprite2D* GetSkill2() { return mSkill2; }
	Odyssey::Sprite2D* GetSkill3() { return mSkill3; }
	Odyssey::Sprite2D* GetSkill4() { return mSkill4; }

	// Skill Backgrounds
	std::vector<Odyssey::Rectangle2D*> GetSkillBackgroundList() { return mSkillBackgroundList; }

	// Status Effect and Buff Elements
	Odyssey::Sprite2D* GetAttackUpBuff() { return mAttackUp; }
	Odyssey::Sprite2D* GetAttackDownBuff() { return mAttackDown; }
	Odyssey::Sprite2D* GetDefenseUpBuff() { return mDefenseUp; }
	Odyssey::Sprite2D* GetDefenseDownBuff() { return mDefenseDown; }
	Odyssey::Sprite2D* GetSpeedUpBuff() { return mSpeedUp; }
	Odyssey::Sprite2D* GetSpeedDownBuff() {return  mSpeedDown; }
	Odyssey::Sprite2D* GetBleedBuff() { return mBleedBuff; }
	Odyssey::Sprite2D* GetStunBuff() { return mStunBuff; }
	Odyssey::Sprite2D* GetProvokeBuff() { return mProvokeBuff; }
	Odyssey::Sprite2D* GetRegenBuff() { return mRegenBuff; }
	Odyssey::Sprite2D* GetShieldBuff() { return mShieldBuff; }

	// Change the different elements
	void ChangePortrait(std::wstring _filename) { mPortrait->setSprite(_filename, (UINT)mPortrait->getDimensions().x, (UINT)mPortrait->getDimensions().y); }
	void ChangeCharacterName(std::wstring _name) { mCharacterName->setText(_name); }
	void ChangeAttackNumber(std::wstring _num) { mAttackNumber->setText(_num); }
	void ChangeDefenseNumber(std::wstring _num) { mDefenseNumber->setText(_num); }
	void ChangeSpeedNumber(std::wstring _num) { mSpeedNumber->setText(_num); }
	void ChangeHealthNumber(std::wstring _text) { mHealthNumber->setText(_text); }
	void ChangeManaNumber(std::wstring _text) { mManaNumber->setText(_text); }

	// Change the skill elements
	void ChangeSkill(Skills* _skill, int _numOfSkill);

	// Change the skill background colors
	void ChangeSkillBackgroundColors(DirectX::XMFLOAT3 _newColor);

	// Clear status effects
	void ClearStatusEffects();

private: //Variables

	// HUD Position
	DirectX::XMFLOAT2 mHudPosition = { 0.0f, 0.0f };

	// HUD Elements
	Odyssey::UICanvas* mCanvas = nullptr;
	Odyssey::Sprite2D* mPortrait = nullptr;
	Odyssey::Rectangle2D* mXpBar = nullptr;
	Odyssey::Text2D* mCharacterName = nullptr;
	Odyssey::Text2D* mLevelNumber = nullptr;
	Odyssey::Text2D* mAttackNumber = nullptr;
	Odyssey::Text2D* mDefenseNumber = nullptr;
	Odyssey::Text2D* mSpeedNumber = nullptr;
	Odyssey::Rectangle2D* mHealthBar = nullptr;
	Odyssey::Text2D* mHealthNumber = nullptr;
	Odyssey::Rectangle2D* mShieldBar = nullptr;
	Odyssey::Rectangle2D* mManaBar = nullptr;
	Odyssey::Text2D* mManaNumber = nullptr;
	Odyssey::Text2D* mTurnNumber = nullptr;
	Odyssey::Rectangle2D* mHudBlocker = nullptr;
	float mBarOpacitySpeed = 5.0f;
	bool mHpBarFadingDown = true;
	bool mMpBarFadingDown = true;

	// Skill Elements
	Odyssey::Sprite2D* mSkill1 = nullptr;
	Odyssey::Sprite2D* mSkill2 = nullptr;
	Odyssey::Sprite2D* mSkill3 = nullptr;
	Odyssey::Sprite2D* mSkill4 = nullptr;

	// Skill Backgrounds
	std::vector<Odyssey::Rectangle2D*> mSkillBackgroundList;
	float mOpacitySpeed = 2.0f;
	bool mFadingDown[4] = { true, true, true, true };

	// Status Effect and Buff Elements
	Odyssey::Sprite2D* mAttackUp = nullptr;
	Odyssey::Sprite2D* mAttackDown = nullptr;
	Odyssey::Sprite2D* mDefenseUp = nullptr;
	Odyssey::Sprite2D* mDefenseDown = nullptr;
	Odyssey::Sprite2D* mSpeedUp = nullptr;
	Odyssey::Sprite2D* mSpeedDown = nullptr;
	Odyssey::Sprite2D* mBleedBuff = nullptr;
	Odyssey::Sprite2D* mStunBuff = nullptr;
	Odyssey::Sprite2D* mProvokeBuff = nullptr;
	Odyssey::Sprite2D* mRegenBuff = nullptr;
	Odyssey::Sprite2D* mShieldBuff = nullptr;

private: // Fucntions
};

