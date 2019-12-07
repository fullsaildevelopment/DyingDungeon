#pragma once
#include "EngineIncludes.h"
#include "Material.h"

namespace Odyssey
{
	class MaterialManager
	{
	public:
		static MaterialManager& getInstance();
		~MaterialManager() { }
	private:
		MaterialManager() { }
	public:
		std::shared_ptr<Material> createMaterial();
		std::shared_ptr<Material> importMaterial(std::fstream& file);
		std::shared_ptr<Material> importMaterial(const char* filename);
		Material* getMaterial(int materialID);
	private:
		typedef std::vector<std::shared_ptr<Material>> MaterialList;
		MaterialList mMaterialList;
	};
}