#include "TextureManager.h"
#include "Texture.h"


namespace Odyssey
{
	TextureManager& TextureManager::getInstance()
	{
		static TextureManager instance;
		return instance;
	}

	int TextureManager::importTexture(TextureType textureType, const char* filename)
	{
		std::string key = filename;
		// Check the texture map for this filename
		if (mTextureMap.count(key) != 0)
		{
			return mTextureMap[key];
		}
		std::string file = filename;
		std::string filePath = texturePath + file;

		// Create the texture and store it in the list
		std::unique_ptr<Texture> texture = std::make_unique<Texture>(textureType, filePath.c_str());
		mTextureList.push_back(std::move(texture));

		// Store the filename and ID kvp in the map for future reference
		int id = static_cast<int>(mTextureList.size()) - 1;
		mTextureMap[key] = id;

		return id;
	}
	Texture* TextureManager::getTexture(int index)
	{
		return mTextureList[index].get();
	}
}