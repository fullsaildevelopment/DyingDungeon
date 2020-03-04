#include "TowerSelectionPrefabFactory.h"

TowerSelectionPrefabFactory& TowerSelectionPrefabFactory::getInstance()
{
	static TowerSelectionPrefabFactory instance;
	return instance;
}

void TowerSelectionPrefabFactory::CreateTowerSelectionPrefab(Odyssey::Application* _application, std::vector<TeamManager::EnemySetups> _enemyEnums)
{
	// Create the prefab object
	Odyssey::Entity* newInfoPopup = _application->createPrefab();
	// Add a canvas
	Odyssey::UICanvas* newCanvas = newInfoPopup->addComponent<Odyssey::UICanvas>();

	// Create starting variables
	UINT width = 500;
	UINT height = 300;
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f }; // Position
	DirectX::XMFLOAT4 color = { 255.0f, 255.0f, 255.0f, 1.0f }; // Color
	Odyssey::TextProperties properties;
	properties.bold = true;
	properties.italic = false;
	properties.fontSize = 25.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	properties.fontName = L"Tw Cen MT Condensed";

	// Create the info popup
	// Background
	position.x = 640.0f - (static_cast<float>(width) / 2.0f);
	position.y = 100.0f;
	color = { 0.0f, 0.0f, 0.0f, 1.0f };
	Odyssey::Rectangle2D* rect = newCanvas->addElement<Odyssey::Rectangle2D>(position, color, width, height);
	rect->setOpacity(0.8f);

	// Add in tower level text
	height = 35;
	position.x += 5.0f;
	position.y += 5.0f;
	properties.fontSize = 25.0f;
	properties.textAlignment = Odyssey::TextAlignment::Left;
	properties.paragraphAlignment = Odyssey::ParagraphAlignment::Top;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	newCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Tower Level: " + std::to_wstring((int)mTowerSelectPopupPrefabs.size() + 1), properties);

	// Add in enemies text and icons
	position.y += static_cast<float>(height) + 10.0f;
	float previousX = position.x;
	width = 120;
	color = { 255.0f, 255.0f, 255.0f, 1.0f };
	newCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Enemies:", properties);
	// Add in the icons
	position.x += static_cast<float>(width);
	width = 64;
	height = 64;

	// For each enemy in the enum list
	for (int i = 0; i < _enemyEnums.size(); i++)
	{
		switch (_enemyEnums[i].pEnemyType)
		{
			case TeamManager::EnemyType::Skeleton:
				newCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/SkeletonPortrait.png", width, height);
				break;
			case TeamManager::EnemyType::CasterDemon:
				newCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/CasterDemonPortrait.png", width, height);
				break;
			case TeamManager::EnemyType::MeleeDemon:
				newCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/MeleeDemonPortrait.png", width, height);
				break;
			case TeamManager::EnemyType::Summoner:
				newCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/SummonerPortrait.png", width, height);
				break;
			case TeamManager::EnemyType::Ganfaul:
				newCanvas->addElement<Odyssey::Sprite2D>(position, L"assets/images/GanfaulPortrait.jpg", width, height);
				break;
			default:
				break;
		}
		
		position.x += static_cast<float>(width) + 10.0f;
	}

	// Set up rewards text
	position.x = previousX;
	position.y += static_cast<float>(height) + 5.0f;
	width = 600;
	height = 35;
	newCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Rewards: Immense amount of FUN!", properties);

	// Set up description
	position.y += static_cast<float>(height) + 20.0f;
	width = 300;
	height = 25;
	properties.fontSize = 15.0f;
	properties.italic = true;
	newCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"Description:", properties);
	position.y += static_cast<float>(height) + 5.0f;
	width = 475;
	height = 275;
	properties.fontSize = 12.0f;
	newCanvas->addElement<Odyssey::Text2D>(position, color, width, height, L"A grand pair of worn statues in a overcast mountain top marks the entrance to this dungeon. Beyond the pair of worn statues lies a grand, humid room. It's covered in remains, ash and ash. Your torch allows you to see carved out openings filled with pottery, worn and ravished by time itself.", properties);
	
	// Decide what popup this is for
	mTowerSelectPopupPrefabs[TowerSelectPopupPrefabs((int)mTowerSelectPopupPrefabs.size())] = newInfoPopup;
}

Odyssey::Entity* TowerSelectionPrefabFactory::GetInfoPrefabs(TowerSelectPopupPrefabs _prefab)
{
	return mTowerSelectPopupPrefabs[_prefab];
}
