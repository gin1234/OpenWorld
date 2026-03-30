#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;
namespace TSEngine {
	class IConfig {
	public:
		virtual IConfig* Init(const std::string Name) = 0;

		virtual ~IConfig() {
		};

		virtual bool Load(std::string Path) = 0;

		std::string GetFileName(){
			return FileName;
		};
		template<typename ObjectType>
		bool LoadObjectFromFile(const std::string& Path,ObjectType& ObjectData) {
			std::ifstream infile(Path);
			nlohmann::json jsonData;

			if (!infile.is_open()) {
				return false;
			}
			infile >> jsonData;
			infile.close();
			ObjectData = jsonData.get<ObjectType>();
			return true;
		};

		template<typename ObjectType, typename CfgType>
		bool LoadConfigFile(CfgType* Cfg, const std::string Name) {
			std::filesystem::path filePath(Name);
			if (std::filesystem::is_directory(filePath)) {
				for (const auto& entry:std::filesystem::directory_iterator(filePath)) {
					if (entry.path().extension() == ".json") {
						ObjectType obj;
						bool ok = LoadObjectFromFile<ObjectType>(entry.path().string(),obj);
						if (!ok) {
							return false;
						}
						Cfg->SetData(obj);
					} 
				}
			}
			else if (std::filesystem::is_regular_file(filePath)) {
				if (filePath.extension() == ".json") {
					bool ok = LoadObjectFromFile<ObjectType>(Name, obj);
					if (!ok) {
						return false;
					}
					Cfg->SetData(obj);
				}
			}
			else {
				return false;
			}
			return true;
		};

	protected:
		std::string FileName;
	};

	struct ConfigTemplate
	{
		std::string id;
	};
}