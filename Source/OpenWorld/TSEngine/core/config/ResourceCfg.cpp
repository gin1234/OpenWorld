#include "ResourceCfg.h"

namespace TSEngine {
	void to_json(json& j, const Resource& p)
	{
		j = json{ {"entityType",p.entityType},{"prefabId",p.prefabId},{"states",p.states}};
	}
	void from_json(const json& j, Resource& p)
	{
		if (j.contains("entityType")) {
			j.at("entityType").get_to(p.entityType);
		}

		if (j.contains("prefabId")) {
			j.at("prefabId").get_to(p.prefabId);
		}

		if (j.contains("states")) {
			j.at("states").get_to(p.states);
		}
	}
	void to_json(json& j, const State& p)
	{
		j = json{ {"dstState",p.dstState},{"srcState",p.srcState},{"resourceStateId",p.resourceStateId} };
	}
	void from_json(const json& j, State& p)
	{
		if (j.contains("dstState")) {
			j.at("dstState").get_to(p.dstState);
		}

		if (j.contains("srcState")) {
			j.at("srcState").get_to(p.srcState);
		}

		if (j.contains("resourceStateId")) {
			j.at("resourceStateId").get_to(p.resourceStateId);
		}
	}
}
