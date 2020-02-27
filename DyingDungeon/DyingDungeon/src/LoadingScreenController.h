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
private:
	const char* mNextScene;
	Odyssey::Entity* mLoadingScreenPrefab;
	Odyssey::Entity* mLoadingScreen;
	Odyssey::Sprite2D* mLogo;
	Odyssey::Text2D* mContinue;
};