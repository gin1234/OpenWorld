#include "MeshResource.h"
namespace TSEngine
{
	void MeshResource::OnStaticMeshLoaded(FSoftObjectPath SoftObjectPath)
	{
		UStaticMesh* StaticMesh = Cast<UStaticMesh>(SoftObjectPath.ResolveObject());
		if (StaticMesh)
		{
			UE_LOG(LogTemp, Log, TEXT("静态网格体加载成功：%s"), *StaticMesh->GetName());
			
		}
	}
	bool MeshResource::Load(Entity* tsEntity, Prefab prefab, FStreamableManager* StreamableManager)
	{	
		if (prefab.path.empty())
		{
			return false;
		}
		FSoftObjectPath SoftPath((prefab.path.data()));
		StreamableManager->RequestAsyncLoad(SoftPath, FStreamableDelegate::CreateUObject(
			this, &MeshResource::OnStaticMeshLoaded, SoftPath
		));
	}

	bool MeshResource::Update(Entity* tsEntity, std::map<std::string, std::string> property)
	{
	}
}
