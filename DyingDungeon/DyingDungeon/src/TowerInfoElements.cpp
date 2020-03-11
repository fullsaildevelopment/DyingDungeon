#include "TowerInfoElements.h"

CLASS_DEFINITION(Component, TowerInfoElements)

std::shared_ptr<Odyssey::Component> TowerInfoElements::clone() const
{
	return std::make_shared<TowerInfoElements>(*this);
}

void TowerInfoElements::initialize()
{
}

void TowerInfoElements::update(double deltaTime)
{
}

void TowerInfoElements::onDestroy()
{
}

void TowerInfoElements::ChangeLevelNumber(std::wstring _newLevelNum)
{
	mLevelNum->setText(L"Tower Level: " + _newLevelNum);
}

void TowerInfoElements::ChangeEnemySprite(int _index, std::wstring _newImageFile)
{
	mEnemySprites[_index]->setSprite(_newImageFile, mEnemySprites[_index]->getDimensions().x, mEnemySprites[_index]->getDimensions().y);
}
