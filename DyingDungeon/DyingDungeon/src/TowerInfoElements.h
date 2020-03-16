#pragma once
#include "Component.h"
#include "UICanvas.h"
#include "Sprite2D.h"
#include "Text2D.h"
#include "Rectangle2D.h"

class TowerInfoElements : public Odyssey::Component
{
	CLASS_DECLARATION(TowerInfoElements)

public:
	TowerInfoElements() = default;
	virtual std::shared_ptr<Odyssey::Component> clone() const;
	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void onDestroy();
public: // Functions

	// Setters
	void SetCanvas(Odyssey::UICanvas* _canvas) { mCanvas = _canvas; }
	void SetLevelNum(Odyssey::Text2D* _levelNum) { mLevelNum = _levelNum; }
	void SetEnemySpriteIntoList(Odyssey::Sprite2D* _sprite) { mEnemySprites.push_back(_sprite); }

	// Getters
	Odyssey::UICanvas* GetCanvas() { return mCanvas; }
	std::vector<Odyssey::Sprite2D*> GetEnemySprites() { return mEnemySprites; }

	// Change Functions
	void ChangeLevelNumber(std::wstring _newLevelNum);
	void ChangeEnemySprite(int _index, std::wstring _newImageFile);

private: // Variables

	// Tower Info Elements
	Odyssey::UICanvas* mCanvas = nullptr;
	Odyssey::Text2D* mLevelNum = nullptr;
	std::vector<Odyssey::Sprite2D*> mEnemySprites;

};