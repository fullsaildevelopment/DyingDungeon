#include "MaterialManager.h"
#include "TextureManager.h"
#include <fstream>

namespace Odyssey
{
	MaterialManager& MaterialManager::getInstance()
	{
		static MaterialManager instance;
		return instance;
	}

	std::shared_ptr<Material> MaterialManager::createMaterial()
	{
		// Make a new material
		std::shared_ptr<Material> material = std::make_shared<Material>();

		// Add the material to the list
		mMaterialList.push_back(material);

		return material;
	}

	Material* MaterialManager::getMaterial(int index)
	{
		return mMaterialList[index].get();
	}
}