#pragma once
#include "OdysseyEngine.h"

class SkillHoverComponent : public Odyssey::Component
{
	CLASS_DECLARATION(SkillHoverComponent)
public:
	virtual void initialize();
	void registerSprite(Odyssey::Sprite2D* sprite, Odyssey::UICanvas* canvas);
	void callback1();
	void callback2();
	void callback3();
	void callback4();
public:
	std::vector<Odyssey::Sprite2D*> mSprites;
	std::vector<Odyssey::UICanvas*> mCanvas;
	std::vector<bool> mStates;
};