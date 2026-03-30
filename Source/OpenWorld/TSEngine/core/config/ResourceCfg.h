#include "Iconfig.h"
#include <map>
#include <list>
namespace TSEngine {
	struct Resource 
	{
		std::string prefabId;
		std::string entityType;
		std::list<State> states;
	};

	struct State
	{
		std::string srcState;
		std::string dstState;
		std::string resourceStateId;
	};

	void to_json(json& j, const Resource& p);

	void from_json(const json& j, Resource& p);

	void to_json(json& j, const State& p);

	void from_json(const json& j, State& p);

	class ResourceCfg : public IConfig {
	public:
		ResourceCfg() = default;
		ResourceCfg(const ResourceCfg&) = delete;
		ResourceCfg& operator=(const ResourceCfg&) = delete;

		static ResourceCfg& GetInstance() {
			static ResourceCfg instance;
			return instance;
		}

		virtual IConfig* Init(const std::string Name) override {
			FileName = Name;
			return this;
		};

		void SetData(Resource Res) {
			prefabDatas[Res.id] = Res;
		}

		virtual bool Load(const std::string Path) override {
			return LoadConfigFile<Resource, ResourceCfg>(this,Path);
		}

		Resource Get(const std::string& Key) {
			return prefabDatas[Key];
		}

	private:
		std::map<std::string, Resource> prefabDatas;
	};
}