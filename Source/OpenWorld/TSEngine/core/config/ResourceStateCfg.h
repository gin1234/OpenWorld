#include "Iconfig.h"
#include <map>
namespace TSEngine {
	struct  ResourceState : public ConfigTemplate
	{
		std::string prefabId;
		std::map<std::string, std::string> property;
	};

	void to_json(json& j, const ResourceState& p);

	void from_json(const json& j, ResourceState& p);

	class ResourceStateCfg : public IConfig {
	public:
		ResourceStateCfg() = default;
		ResourceStateCfg(const ResourceStateCfg&) = delete;
		ResourceStateCfg& operator=(const ResourceStateCfg&) = delete;

		static ResourceStateCfg& GetInstance() {
			static ResourceStateCfg instance;
			return instance;
		}

		virtual IConfig* Init(const std::string Name) override {
			FileName = Name;
			return this;
		};

		void SetData(const ResourceState& Prefab) {
			prefabDatas[Prefab.id] = Prefab;
		}

		virtual bool Load(const std::string Path) override {
			return LoadConfigFile<ResourceState, ResourceStateCfg>(this,Path);
		}

		ResourceState Get(const std::string& Key) {
			return prefabDatas[Key];
		}

	private:
		std::map<std::string, ResourceState> prefabDatas;
	};
}