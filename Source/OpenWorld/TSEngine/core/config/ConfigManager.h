#pragma once
#include "Iconfig.h"
namespace TSEngine {
	class ConfigManager {
	public:
		ConfigManager(const ConfigManager&) = delete;
		ConfigManager& operator=(const ConfigManager&) = delete;

		static ConfigManager* getInstance() {
			static ConfigManager* instance;
			return instance;
		};

		void LoadAll();

		void Register(IConfig* cfg);

		void Start();

	private:
		std::string basePath;
		std::string projectPath;
		std::vector<IConfig*> configs;

		std::string env_class_name = "Env";
	};
}