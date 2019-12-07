#include "MaterialManager.h"
#include "BufferManager.h"
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

	std::shared_ptr<Material> MaterialManager::importMaterial(std::fstream& file)
	{
		// Create a mat file import object
		MatFileImport matFile;

		// Resize the arrays to hold 4 textures
		matFile.texFilenames.resize(4);
		matFile.texColors.resize(4);
		matFile.texFactors.resize(4);

		// Iterate through all 4 possible textures
		for (int i = 0; i < 4; i++)
		{
			// Read in if the material has this texture
			uint64_t hasTex;
			file.read((char*)&hasTex, sizeof(uint64_t));

			// Texture was not serialized, move on
			if (hasTex == 0)
				continue;

			// Get the size of the filename and allocate an array to hold it
			uint64_t size;
			file.read((char*)&size, sizeof(uint64_t));
			matFile.texFilenames[i] = new char[size];

			// Read in the filename, color and factor of the texture
			file.read((char*)matFile.texFilenames[i], size * sizeof(char));

			// Read in colors and factors for each texture except the normal
			if (i != 3)
			{
				file.read((char*)&matFile.texColors[i], sizeof(DirectX::XMFLOAT3));
				file.read((char*)&matFile.texFactors[i], sizeof(float));
			}
		}

		// Create a blank material
		std::shared_ptr<Material> mat = createMaterial();

		// Convert the diffuse color from the import to an XMFloat4
		DirectX::XMFLOAT4 diffuseColor = { matFile.texColors[0].x, matFile.texColors[0].y, matFile.texColors[0].z, 1.0f };
		// Set the diffuse color in the material
		mat->setDiffuseColor(diffuseColor);

		// Generate the textures from the material file
		for (int i = 0; i < 4; i++)
		{
			// Check if the texture has a filename
			if (matFile.texFilenames[i])
			{
				// Create the texture and set it in the material
				int texID = TextureManager::getInstance().importTexture((TextureType)i, matFile.texFilenames[i]);
				mat->setTexture((TextureType)i, texID);
			}
		}

		return mat;
	}

	std::shared_ptr<Material> MaterialManager::importMaterial(const char* filename)
	{
		// Open the mesh file
		std::fstream file{ filename, std::ios_base::in | std::ios_base::binary };

		// Check if the file is open
		assert(file.is_open());

		// Import the material using the file pointer
		return importMaterial(file);
	}

	Material* MaterialManager::getMaterial(int index)
	{
		return mMaterialList[index].get();
	}
}