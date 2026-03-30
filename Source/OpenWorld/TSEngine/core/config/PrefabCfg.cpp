#include "PrefabCfg.h"

namespace TSEngine {
	void to_json(json& j, const Prefab& p)
	{
		j = json{ {"prefabType",p.prefabType},{"path",p.path},{"id",p.id} };
	}
	void from_json(const json& j, Prefab& p)
	{
		if (j.contains("prefabType")) {
			j.at("prefabType").get_to(p.prefabType);
		}

		if (j.contains("path")) {
			j.at("path").get_to(p.path);
		}

		if (j.contains("id")) {
			j.at("id").get_to(p.id);
		}

	}
}