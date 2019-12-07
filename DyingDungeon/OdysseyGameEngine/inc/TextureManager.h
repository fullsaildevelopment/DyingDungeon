#pragma once
#include "EngineIncludes.h"
#include "Texture.h"

namespace Odyssey
{
	class TextureManager
	{
	public: // Singleton implementation
		static TextureManager& getInstance();
		~TextureManager() { }
	private: // Private constructor
		TextureManager() { }
	public: // Public functions
		int importTexture(TextureType textureType, const char* filename);
		Texture* getTexture(int index);
	private: // Private member variables
		typedef std::vector<std::unique_ptr<Texture>> TextureList;
		TextureList mTextureList;
		std::map<std::string, int> mTextureMap;
		std::string texturePath = "Assets\\textures\\";
	};
}