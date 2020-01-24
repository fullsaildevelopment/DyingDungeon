#include "SkillHoverComponent.h"

CLASS_DEFINITION(Odyssey::Component, SkillHoverComponent)
void SkillHoverComponent::initialize()
{
	mSprites[0]->registerCallback("onMouseEnter", this, &SkillHoverComponent::callback1);
	mSprites[1]->registerCallback("onMouseEnter", this, &SkillHoverComponent::callback2);
	mSprites[2]->registerCallback("onMouseEnter", this, &SkillHoverComponent::callback3);
	mSprites[3]->registerCallback("onMouseEnter", this, &SkillHoverComponent::callback4);
	mSprites[0]->registerCallback("onMouseExit", this, &SkillHoverComponent::callback1);
	mSprites[1]->registerCallback("onMouseExit", this, &SkillHoverComponent::callback2);
	mSprites[2]->registerCallback("onMouseExit", this, &SkillHoverComponent::callback3);
	mSprites[3]->registerCallback("onMouseExit", this, &SkillHoverComponent::callback4);
	mStates.push_back(false);
	mStates.push_back(false);
	mStates.push_back(false);
	mStates.push_back(false);
}

void SkillHoverComponent::registerSprite(Odyssey::Sprite2D* sprite, Odyssey::UICanvas* canvas)
{
	mSprites.push_back(sprite);
	mCanvas.push_back(canvas);
}

void SkillHoverComponent::callback1()
{
	mStates[0] = !mStates[0];
	mCanvas[0]->setActive(mStates[0]);
}

void SkillHoverComponent::callback2()
{
	mStates[1] = !mStates[1];
	mCanvas[1]->setActive(mStates[1]);
}

void SkillHoverComponent::callback3()
{
	mStates[2] = !mStates[2];
	mCanvas[2]->setActive(mStates[2]);
}

void SkillHoverComponent::callback4()
{
	mStates[3] = !mStates[3];
	mCanvas[3]->setActive(mStates[3]);
}