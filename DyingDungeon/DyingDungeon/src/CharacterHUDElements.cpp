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
			pSkill1->setSprite(_skill->GetSkillIconPath());
			// Set up the skill's popup canvas
			SetupSkillHover(pSkill1Popup, _skill);
			break;
		case 2:
			// Set the new sprite image
			pSkill2->setSprite(_skill->GetSkillIconPath());
			// Set up the skill's popup canvas
			SetupSkillHover(pSkill2Popup, _skill);
			break;
		case 3:
			// Set the new sprite image
			pSkill3->setSprite(_skill->GetSkillIconPath());
			// Set up the skill's popup canvas
			SetupSkillHover(pSkill3Popup, _skill);
			break;
		case 4:
			// Set the new sprite image
			pSkill4->setSprite(_skill->GetSkillIconPath());
			// Set up the skill's popup canvas
			SetupSkillHover(pSkill4Popup, _skill);
			break;
		default:
			break;
	}
}

void CharacterHUDElements::SetupSkillHover(SkillPopup* _skillPopup, Skills* _skill)
{
	// Get the canvas
	Odyssey::UICanvas* canvas = _skillPopup->pCanvas;
	
	// Set color
	DirectX::XMFLOAT4 themeColor = DirectX::XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f);
	
	UINT windowWidth = 1280;
	UINT windowHeight = 720;
	float x = pHudPosition.x;
	float y = pHudPosition.y - 130.0f;
	UINT width = 300;
	UINT height = 115;
	UINT pad = 10;
	
	Odyssey::TextProperties title;
	title.bold = true;
	title.italic = false;
	title.fontSize = 24.0f;
	title.textAlignment = Odyssey::TextAlignment::Center;
	title.paragraphAlignment = Odyssey::ParagraphAlignment::Center;
	title.fontName = L"Tw Cen MT Condensed";
	
	Odyssey::TextProperties properties;
	properties.bold = false;
	properties.italic = false;
	properties.fontSize = 14.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	properties.fontName = L"Tw Cen MT Condensed";
	
	// Background and Separators
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y), DirectX::XMFLOAT4(50.5f, 50.5f, 50.5f, 0.75f), width, height);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x, y + 40), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), width, 2);
	
	// Title Text and Icons
	_skillPopup->pSkillName = canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + 40, y), themeColor, width - 80, 40, _skill->GetSkillName(), title);
	_skillPopup->pSkillImage = canvas->addElement<Odyssey::Sprite2D>(DirectX::XMFLOAT2(x, y), _skill->GetSkillIconPath(), 40, 40);
	canvas->addElement<Odyssey::Rectangle2D>(DirectX::XMFLOAT2(x + width - 40, y), DirectX::XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f), 40, 40);
	_skillPopup->pSkillManaCost = canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + width - 40, y), DirectX::XMFLOAT4(0.0f, 122.5f, 122.5f, 1.0f), 40, 40, std::to_wstring(_skill->GetManaCost()), title);
	
	// Description
	_skillPopup->pSkillDescription = canvas->addElement<Odyssey::Text2D>(DirectX::XMFLOAT2(x + pad, y + 45), themeColor, width - (2 * pad), 85, _skill->GetSkillDescription(), properties);
	
	// Turn the canvas off on start
	canvas->setActive(false);
}
