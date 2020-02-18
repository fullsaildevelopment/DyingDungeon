#pragma once
#include "Component.h"
#include "UICanvas.h"
#include "Sprite2D.h"
#include "Text2D.h"
#include "Rectangle2D.h"

class SkillHUDElements : public Odyssey::Component
{
	CLASS_DECLARATION(SkillHUDElements)

public:
	SkillHUDElements() = default;
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestroy();
public:
	// Setters

	// HUD Position
	void SetHudPosition(DirectX::XMFLOAT2 _hudPos) { pHudPosition = _hudPos; }

	// HUD Elements
	void SetCanvas(Odyssey::UICanvas* _canvas) { mCanvas = _canvas; }
	void SetSkillImage(Odyssey::Sprite2D* _sprite) { mSkillImage = _sprite; }
	void SetSkillName(Odyssey::Text2D* _text) { mSkillName = _text; }
	void SetManaCost(Odyssey::Text2D* _text) { mManaCost = _text; }
	void SetSkillDescription(Odyssey::Text2D* _text) { mSkillDescription = _text; }

	// Getters

	// HUD Elements
	Odyssey::UICanvas* GetCanvas() { return mCanvas; }
	Odyssey::Sprite2D* GetSkillImage() { return mSkillImage; }
	Odyssey::Text2D* GetSkillName() { return mSkillName; }
	Odyssey::Text2D* GetManaCost() { return mManaCost; }
	Odyssey::Text2D* GetSkillDescription() { return mSkillDescription; }

	// Changers

	// Change the different elements
	void ChangeSkillImage(std::wstring _filename) { mSkillImage->setSprite(_filename, mSkillImage->getDimensions().x, mSkillImage->getDimensions().y); }
	void ChangeSkillName(std::wstring _name) { mSkillName->setText(_name); }
	void ChangeManaCost(std::wstring _num) { mManaCost->setText(_num); }
	void ChangeSkillDescription(std::wstring _desc) { mSkillDescription->setText(_desc); }

	// Change the theme color text for all of the text elements
	void ChangeThemeColor(DirectX::XMFLOAT3 _themeColor);

private: //Variables

	// HUD Position
	DirectX::XMFLOAT2 pHudPosition;

	// HUD Elements
	Odyssey::UICanvas* mCanvas;
	Odyssey::Sprite2D* mSkillImage;
	Odyssey::Text2D* mSkillName;
	Odyssey::Text2D* mManaCost;
	Odyssey::Text2D* mSkillDescription;

private: // Fucntions
};

