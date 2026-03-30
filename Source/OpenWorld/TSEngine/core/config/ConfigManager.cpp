#include "ConfigManager.h"

namespace TSEngine {
	void ConfigManager::LoadAll()
	{
		for (IConfig* cfg : configs) {
			if (strcmp(typeid(*cfg).name(),this->env_class_name.c_str()) == 0) {
				cfg->Load(this->projectPath + "\\" + cfg->GetFileName());
				continue;
			}
			cfg->Load(basePath + "\\" + cfg->GetFileName());
		}

	}
	void ConfigManager::Register(IConfig* cfg)
	{
		configs.push_back(cfg);
	}
	void ConfigManager::Start()
	{
		LoadAll();
	}
}