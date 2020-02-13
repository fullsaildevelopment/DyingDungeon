#pragma once
#include "Component.h"
#include "UICanvas.h"
#include "Sprite2D.h"
#include "Text2D.h"
#include "Rectangle2D.h"

class CharacterHUDElements : public Odyssey::Component
{
	CLASS_DECLARATION(CharacterHUDElements)

	struct SkillPopup
	{
		Odyssey::UICanvas* pCanvas;
		Odyssey::Text2D* pSkillName;
		Odyssey::Sprite2D* pSkillImage;
		Odyssey::Text2D* pSkillManaCost;
		Odyssey::Text2D* pSkillDescription;
	};

public:
	CharacterHUDElements() = default;
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestroy();
public:
	// Setters

	// HUD Elements
	void SetCanvas(Odyssey::UICanvas* _canvas) { pCanvas = _canvas; }
	void SetPortrait(Odyssey::Sprite2D* _sprite) { pPortrait = _sprite; }
	void SetXpBar(Odyssey::Rectangle2D* _rect) { pXpBar = _rect; }
	void SetCharacterName(Odyssey::Text2D* _text) { pCharacterName = _text; }
	void SetLevelNumber(Odyssey::Text2D* _text) { pLevelNumber = _text; }
	void SetAttackNumber(Odyssey::Text2D* _text) { pAttackNumber = _text; }
	void SetDefenseNumber(Odyssey::Text2D* _text) { pDefenseNumber = _text; }
	void SetSpeedNumber(Odyssey::Text2D* _text) { pSpeedNumber = _text; }
	void SetHealthBar(Odyssey::Rectangle2D* _rect) { pManaBar = _rect; }
	void SetHealthNumber(Odyssey::Text2D* _text) { pHealthNumber = _text; }
	void SetManaBar(Odyssey::Rectangle2D* _rect) { pManaBar = _rect; }
	void SetManaNumber(Odyssey::Text2D* _text) { pManaNumber = _text; }
	void SetTurnNumber(Odyssey::Text2D* _text) { pTurnNumber = _text; }

	// Skill Elements
	void SetSkill1(Odyssey::Sprite2D* _sprite) { pSkill1 = _sprite; }
	void SetSkill2(Odyssey::Sprite2D* _sprite) { pSkill2 = _sprite; }
	void SetSkill3(Odyssey::Sprite2D* _sprite) { pSkill3 = _sprite; }
	void SetSkill4(Odyssey::Sprite2D* _sprite) { pSkill4 = _sprite; }
	void SetSkill1Popup(SkillPopup* _skillPopup) { pSkill1Popup = _skillPopup; }
	void SetSkill2Popup(SkillPopup* _skillPopup) { pSkill2Popup = _skillPopup; }
	void SetSkill3Popup(SkillPopup* _skillPopup) { pSkill3Popup = _skillPopup; }
	void SetSkill4Popup(SkillPopup* _skillPopup) { pSkill4Popup = _skillPopup; }

	// Getters

	// HUD Elements
	Odyssey::UICanvas* GetCanvas() { return pCanvas; }
	Odyssey::Sprite2D* GetPortrait() { return pPortrait; }
	Odyssey::Rectangle2D* GetXpBar() { return pXpBar; }
	Odyssey::Text2D* GetCharacterName() { return pCharacterName; }
	Odyssey::Text2D* GetLevelNumber() { return pLevelNumber; }
	Odyssey::Text2D* GetAttackNumber() { return pAttackNumber; }
	Odyssey::Text2D* GetDefenseNumber() { return pDefenseNumber; }
	Odyssey::Text2D* GetSpeedNumber() { return pSpeedNumber; }
	Odyssey::Rectangle2D* GetHealthBar() { return pManaBar; }
	Odyssey::Text2D* GetHealthNumber() { return pHealthNumber; }
	Odyssey::Rectangle2D* GetManaBar() { return pManaBar; }
	Odyssey::Text2D* GetManaNumber() { return pManaNumber; }
	Odyssey::Text2D* GetTurnNumber() { return pTurnNumber; }

	// Skills Elements
	Odyssey::Sprite2D* GetSkill1() { return pSkill1; }
	Odyssey::Sprite2D* GetSkill2() { return pSkill2; }
	Odyssey::Sprite2D* GetSkill3() { return pSkill3; }
	Odyssey::Sprite2D* GetSkill4() { return pSkill4; }
	SkillPopup* GetSkill1Popup() { return pSkill1Popup; }
	SkillPopup* GetSkill2Popup() { return pSkill2Popup; }
	SkillPopup* GetSkill3Popup() { return pSkill3Popup; }
	SkillPopup* GetSkill4Popup() { return pSkill4Popup; }

private: //Variables

	Odyssey::UICanvas* pCanvas;
	Odyssey::Sprite2D* pPortrait;
	Odyssey::Rectangle2D* pXpBar;
	Odyssey::Text2D* pCharacterName;
	Odyssey::Text2D* pLevelNumber;
	Odyssey::Text2D* pAttackNumber;
	Odyssey::Text2D* pDefenseNumber;
	Odyssey::Text2D* pSpeedNumber;
	Odyssey::Rectangle2D* pHealthBar;
	Odyssey::Text2D* pHealthNumber;
	Odyssey::Rectangle2D* pManaBar;
	Odyssey::Text2D* pManaNumber;
	Odyssey::Text2D* pTurnNumber;

	// Skills
	Odyssey::Sprite2D* pSkill1;
	Odyssey::Sprite2D* pSkill2;
	Odyssey::Sprite2D* pSkill3;
	Odyssey::Sprite2D* pSkill4;
	SkillPopup* pSkill1Popup;
	SkillPopup* pSkill2Popup;
	SkillPopup* pSkill3Popup;
	SkillPopup* pSkill4Popup;

	// Status Effects
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;

private: // Fucntions
};

