#include "StaticMeshHandler.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "../StaticResourceHandler.h"
#include "../PropertyParser.h"

namespace TSEngine {

static TStaticResourceHandler<StaticMeshHandler> _Register("StaticMesh");

StaticMeshHandler::StaticMeshHandler() = default;

void StaticMeshHandler::LoadAsync(const FSoftObjectPath& Path,
                             FResourceLoadedCallback Callback) {
    PendingLoads[Path] = Callback;

    FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
        this, &StaticMeshHandler::OnMeshLoaded, Path, Callback);

    ResourceManager::GetInstance()->GetStreamableManager()->RequestAsyncLoad(
        Path, Delegate);
}

void StaticMeshHandler::OnMeshLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback) {
    UObject* LoadedObject = Path.ResolveObject();

    auto it = PendingLoads.find(Path);
    if (it != PendingLoads.end()) {
        FResourceLoadedCallback CB = it->second;
        PendingLoads.erase(it);

        if (LoadedObject) {
            if (CB.OnLoaded) {
                CB.OnLoaded(LoadedObject);
            }
        } else {
            if (CB.OnFailed) {
                CB.OnFailed();
            }
        }
    }
}

AActor* StaticMeshHandler::CreateActor(Entity* TsEntity, UObject* LoadedObject,
                                  const std::map<std::string, std::string>& Properties) {
    UStaticMesh* StaticMesh = Cast<UStaticMesh>(LoadedObject);
    if (!StaticMesh) {
        UE_LOG(LogTemp, Error, TEXT("StaticMeshHandler: Loaded object is not a UStaticMesh"));
        return nullptr;
    }

    UWorld* World = nullptr;
    for (const FWorldContext& Context : GEngine->GetWorldContexts()) {
        if (Context.WorldType == EWorldType::Game ||
            Context.WorldType == EWorldType::Editor) {
            World = Context.World;
            break;
        }
    }

    if (!World) {
        UE_LOG(LogTemp, Error, TEXT("StaticMeshHandler: Could not find valid World"));
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = *FString::Printf(TEXT("StaticMesh_%d"), TsEntity->ID);
    SpawnParams.Owner = nullptr;
    SpawnParams.Instigator = nullptr;

    FVector Location = FVector::ZeroVector;
    FRotator Rotation = FRotator::ZeroRotator;
    FVector Scale = FVector::OneVector;

    auto it = Properties.find("Location");
    if (it != Properties.end()) {
        Location = PropertyParser::ParseVector(it->second);
    }
    it = Properties.find("Rotation");
    if (it != Properties.end()) {
        Rotation = PropertyParser::ParseRotator(it->second);
    }
    it = Properties.find("Scale");
    if (it != Properties.end()) {
        Scale = PropertyParser::ParseVector(it->second);
    }

    AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(
        AStaticMeshActor::StaticClass(), Location, Rotation, SpawnParams);

    if (!Actor) {
        UE_LOG(LogTemp, Error, TEXT("StaticMeshHandler: Failed to spawn AStaticMeshActor"));
        return nullptr;
    }

    Actor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
    Actor->SetActorScale3D(Scale);

    ApplyProperties(Actor, Properties);

    UE_LOG(LogTemp, Log, TEXT("StaticMeshHandler: Successfully created actor %s for entity %d"),
           *Actor->GetName(), TsEntity->ID);

    return Actor;
}

void StaticMeshHandler::ApplyProperties(UObject* Target,
                                   const std::map<std::string, std::string>& Properties) {
    AStaticMeshActor* Actor = Cast<AStaticMeshActor>(Target);
    if (!Actor) return;

    for (const auto& [Key, Value] : Properties) {
        if (Key == "Location") {
            Actor->SetActorLocation(PropertyParser::ParseVector(Value));
        } else if (Key == "Rotation") {
            Actor->SetActorRotation(PropertyParser::ParseRotator(Value));
        } else if (Key == "Scale") {
            Actor->SetActorScale3D(PropertyParser::ParseVector(Value));
        }
    }
}

} // namespace TSEngine
