#include "EnvCfg.h"
namespace TSEngine {
	void to_json(json& j, const Env& p)
	{
		j = json{ {"id",p.id},{"srcState",p.configPath}};

	}
	void from_json(const json& j, Env& p)
	{
		if (j.contains("id")) {
			j.at("id").get_to(p.id);
		}

		if (j.contains("configPath")) {
			j.at("configPath").get_to(p.configPath);
		}
	}
}