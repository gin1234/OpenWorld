#pragma once
#include "Iconfig.h"
#include <map>
namespace TSEngine {
	struct  Prefab : public ConfigTemplate
	{
		std::string prefabType;
		std::string path;
	};

	void to_json(json& j, const Prefab& p);

	void from_json(const json& j, Prefab& p);

	class PrefabCfg : public IConfig {
	public:
		PrefabCfg() = default;
		PrefabCfg(const PrefabCfg&) = delete;
		PrefabCfg& operator=(const PrefabCfg&) = delete;

		static PrefabCfg& GetInstance() {
			static PrefabCfg instance;
			return instance;
		}

		virtual IConfig* Init(const std::string Name) override {
			FileName = Name;
			return this;
		};

		void SetData(const Prefab& Prefab) {
			prefabDatas[Prefab.id] = Prefab;
		}

		virtual bool Load(const std::string Path) override {
			return LoadConfigFile<Prefab, PrefabCfg>(this,Path);
		}

		Prefab Get(const std::string& Key) {
			return prefabDatas[Key];
		}

	private:
		std::map<std::string, Prefab> prefabDatas;
	};
}