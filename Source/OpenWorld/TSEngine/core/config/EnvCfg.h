#include "Iconfig.h"
#include <map>
namespace TSEngine {
	struct  Env : public ConfigTemplate
	{
		std::string configPath;
	};

	void to_json(json& j, const Env& p);

	void from_json(const json& j, Env& p);

	class EnvCfg : public IConfig {
	public:
		EnvCfg() = default;
		EnvCfg(const EnvCfg&) = delete;
		EnvCfg& operator=(const EnvCfg&) = delete;

		static EnvCfg& getInstance() {
			static EnvCfg instance;
			return instance;
		}

		IConfig* Init(std::string name) override {
			FileName = name;
			return this;
		};

		void SetData(Env prefab) {
			prefabDatas[prefab.id] = prefab;
		}

		bool Load(std::string path) override {
			return LoadConfigFile<Env, EnvCfg>(this,path);
		}

		Env Get(std::string key) {
			return prefabDatas[key];
		}

	private:
		std::map<std::string, PreEnvfab> prefabDatas;
	};
}