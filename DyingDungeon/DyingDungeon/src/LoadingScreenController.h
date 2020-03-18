#pragma once
#include "Component.h"
#include "Event.h"
#include "XTime.h"

class SpawnLoadingScreenEvent : public Odyssey::Event
{
public:
	const char* nextScene;
	SpawnLoadingScreenEvent(const char* scene) : nextScene(scene)
	{

	}
};

class LoadingScreenController : public Odyssey::Component
{
	CLASS_DECLARATION(LoadingScreenController)

public:
	LoadingScreenController();
	virtual void initialize();
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void update(double deltaTime);
	virtual void onDestroy();
private:
	float mCurrentTime;
	float mWaitTime;
	void onSpawnLoadingScreen(SpawnLoadingScreenEvent* evnt);
	void constructLoadingScreen();
	void randomizeLoadingScreen();
private:
	const char* mNextScene;
	Odyssey::Entity* mLoadingScreenPrefab;
	Odyssey::Entity* mLoadingScreen;
	Odyssey::Sprite2D* mLogo;
	Odyssey::Text2D* mContinue;
	std::wstring titleList[6];
	std::wstring backgroundList[6];
	std::wstring spriteList[6];
	std::wstring biographyList[6];
	std::wstring skill1TextList[6];
	std::wstring skill2TextList[6];
	std::wstring skill3TextList[6];
	std::wstring skill4TextList[6];
	std::wstring skill1IconList[6];
	std::wstring skill2IconList[6];
	std::wstring skill3IconList[6];
	std::wstring skill4IconList[6];

	/*
	text:
	0 - title
	1 - bio
	2 - health
	3 - mana
	4 - attack
	5 - defense
	6 - speed
	7 - s1 text
	8 - s2 text
	9 - s3 text
	10 - s4 text
	*/
};