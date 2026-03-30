#include "Resource.h"

namespace TSEngine
{
	class MeshResource : ResourceBase
	{
	public:
		void OnStaticMeshLoaded(FSoftObjectPath SoftObjectPath);
		virtual bool Load(Entity* tsEntity, Prefab prefab, FStreamableManager* StreamableManager) override;
		virtual bool Update(Entity* tsEntity, std::map<std::string, std::string> property) override;
	};
}
