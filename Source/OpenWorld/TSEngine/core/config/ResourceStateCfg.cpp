#include "ResourceStateCfg.h"

namespace TSEngine {
	void to_json(json& j, const ResourceState& p)
	{
		j = json{ {"prefabId",p.prefabId},{"id",p.id},{"property",p.property} };

	}
	void from_json(const json& j, ResourceState& p)
	{
		if (j.contains("prefabId")) {
			j.at("prefabId").get_to(p.prefabId);
		}

		if (j.contains("id")) {
			j.at("id").get_to(p.id);
		}

		if (j.contains("property")) {
			j.at("property").get_to(p.property);
		}
	}
}
