#pragma once
#include "Component.h"

class BattleInstance;

enum class TutorialState { NONE = -1, STAGEONE, STAGETWO, STAGETHREE, STAGEFOUR, STAGEFIVE, STAGESIX};


class TutorialLevel : public Odyssey::Component
{
	CLASS_DECLARATION(TutorialLevel);

public: // Constructors
	TutorialLevel();
	virtual ~TutorialLevel();

public: // Functions

	// Necessary
	virtual void initialize();
	virtual void update(double deltaTime);

private:
	Odyssey::Entity* mPlayer;
	Odyssey::Entity* mEnemy;

	TutorialState currentState;

	void SetupLevel();
	void GoToMainMenu();

	// Asset Paths
	std::map<std::string, std::wstring> HighlightPaths;
};