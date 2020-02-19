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
	void SetHudPosition(DirectX::XMFLOAT2 _hudPos) { pHudPosition = _hudPos; }

	// HUD Elements
	void SetCanvas(Odyssey::UICanvas* _canvas) { pCanvas = _canvas; }
	void SetPortrait(Odyssey::Sprite2D* _sprite) { pPortrait = _sprite; }
	void SetCharacterName(Odyssey::Text2D* _text) { pCharacterName = _text; }
	void SetXpBar(Odyssey::Rectangle2D* _rect) { pXpBar = _rect; }
	void SetLevelNumber(Odyssey::Text2D* _text) { pLevelNumber = _text; }
	void SetAttackNumber(Odyssey::Text2D* _text) { pAttackNumber = _text; }
	void SetDefenseNumber(Odyssey::Text2D* _text) { pDefenseNumber = _text; }
	void SetSpeedNumber(Odyssey::Text2D* _text) { pSpeedNumber = _text; }
	void SetHealthBar(Odyssey::Rectangle2D* _rect) { pHealthBar = _rect; }
	void SetHealthNumber(Odyssey::Text2D* _text) { pHealthNumber = _text; }
	void SetManaBar(Odyssey::Rectangle2D* _rect) { pManaBar = _rect; }
	void SetManaNumber(Odyssey::Text2D* _text) { pManaNumber = _text; }
	void SetTurnNumber(Odyssey::Text2D* _text) { pTurnNumber = _text; }

	// Skill Elements
	void SetSkill1(Odyssey::Sprite2D* _sprite) { pSkill1 = _sprite; }
	void SetSkill2(Odyssey::Sprite2D* _sprite) { pSkill2 = _sprite; }
	void SetSkill3(Odyssey::Sprite2D* _sprite) { pSkill3 = _sprite; }
	void SetSkill4(Odyssey::Sprite2D* _sprite) { pSkill4 = _sprite; }

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
	Odyssey::Rectangle2D* GetHealthBar() { return pHealthBar; }
	Odyssey::Text2D* GetHealthNumber() { return pHealthNumber; }
	Odyssey::Rectangle2D* GetManaBar() { return pManaBar; }
	Odyssey::Text2D* GetManaNumber() { return pManaNumber; }
	Odyssey::Text2D* GetTurnNumber() { return pTurnNumber; }

	// Skills Elements
	Odyssey::Sprite2D* GetSkill1() { return pSkill1; }
	Odyssey::Sprite2D* GetSkill2() { return pSkill2; }
	Odyssey::Sprite2D* GetSkill3() { return pSkill3; }
	Odyssey::Sprite2D* GetSkill4() { return pSkill4; }

	// Change the different elements
	void ChangePortrait(std::wstring _filename) { pPortrait->setSprite(_filename, pPortrait->getDimensions().x, pPortrait->getDimensions().y); }
	void ChangeCharacterName(std::wstring _name) { pCharacterName->setText(_name); }
	void ChangeAttackNumber(std::wstring _num) { pAttackNumber->setText(_num); }
	void ChangeDefenseNumber(std::wstring _num) { pDefenseNumber->setText(_num); }
	void ChangeSpeedNumber(std::wstring _num) { pSpeedNumber->setText(_num); }
	void ChangeHealthNumber(std::wstring _text) { pHealthNumber->setText(_text); }
	void ChangeManaNumber(std::wstring _text) { pManaNumber->setText(_text); }

	// Change the skill elements
	void ChangeSkill(Skills* _skill, int _numOfSkill);

private: //Variables

	// HUD Position
	DirectX::XMFLOAT2 pHudPosition;

	// HUD Elements
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

	// Status Effects
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;
	//Odyssey::Sprite2D* pAttackUp;

private: // Fucntions
};

