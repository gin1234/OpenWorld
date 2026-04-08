#include "SkeletalMeshHandler.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Animation/AnimInstance.h"
#include "Skeleton.h"
#include "../StaticResourceHandler.h"
#include "../ResourceManager.h"
#include "../PropertyParser.h"

namespace TSEngine {

static TStaticPlayableResourceHandler<SkeletalMeshHandler> _RegisterPlayable("SkeletalMesh");

SkeletalMeshHandler::SkeletalMeshHandler() = default;

void SkeletalMeshHandler::LoadAsync(const FSoftObjectPath& Path,
                             FResourceLoadedCallback Callback) {
    PendingLoads[Path] = Callback;

    FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
        this, &SkeletalMeshHandler::OnMeshLoaded, Path, Callback);

    ResourceManager::GetInstance()->GetStreamableManager()->RequestAsyncLoad(
        Path, Delegate);
}

void SkeletalMeshHandler::OnMeshLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback) {
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

AActor* SkeletalMeshHandler::CreateActor(Entity* TsEntity, UObject* LoadedMesh,
                                const std::map<std::string, std::string>& Properties) {
    USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(LoadedMesh);
    if (!SkeletalMesh) {
        UE_LOG(LogTemp, Error, TEXT("SkeletalMeshHandler: Loaded object is not a USkeletalMesh"));
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
        UE_LOG(LogTemp, Error, TEXT("SkeletalMeshHandler: Could not find valid World"));
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = *FString::Printf(TEXT("SkeletalMesh_%d"), TsEntity->ID);
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

    ASkeletalMeshActor* Actor = World->SpawnActor<ASkeletalMeshActor>(
        ASkeletalMeshActor::StaticClass(), Location, Rotation, SpawnParams);

    if (!Actor) {
        UE_LOG(LogTemp, Error, TEXT("SkeletalMeshHandler: Failed to spawn ASkeletalMeshActor"));
        return nullptr;
    }

    USkeletalMeshComponent* MeshComponent = Actor->GetSkeletalMeshComponent();
    if (MeshComponent) {
        MeshComponent->SetSkeletalMesh(SkeletalMesh);
        MeshComponent->SetWorldScale3D(Scale);
    }

    ApplyProperties(Actor, Properties);

    UE_LOG(LogTemp, Log, TEXT("SkeletalMeshHandler: Successfully created actor %s for entity %d"),
           *Actor->GetName(), TsEntity->ID);

    return Actor;
}

USkeletalMeshComponent* SkeletalMeshHandler::GetMeshComponent(AActor* Actor) {
    if (!Actor) return nullptr;
    return Actor->FindComponentByClass<USkeletalMeshComponent>();
}

UAnimInstance* SkeletalMeshHandler::GetAnimInstance(AActor* Actor) {
    USkeletalMeshComponent* MeshComponent = GetMeshComponent(Actor);
    if (!MeshComponent) return nullptr;
    return MeshComponent->GetAnimInstance();
}

void SkeletalMeshHandler::Play(Entity* TsEntity, UObject* Target,
                     const std::map<std::string, std::string>& Params) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) {
        // If Target is the SkeletalMesh itself, find the actor via entity
        UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshHandler::Play - Target is not AActor"));
        return;
    }

    USkeletalMeshComponent* MeshComponent = GetMeshComponent(Actor);
    if (!MeshComponent) {
        UE_LOG(LogTemp, Error, TEXT("SkeletalMeshHandler::Play - No SkeletalMeshComponent"));
        return;
    }

    FAnimationParams AnimParams = PropertyParser::ParseAnimationParams(Params);

    if (AnimParams.AnimationName != NAME_None) {
        UAnimSequenceBase* Animation = FindObject<UAnimSequenceBase>(nullptr, *AnimParams.AnimationName.ToString());
        if (Animation) {
            MeshComponent->PlayAnimation(Animation, AnimParams.bLooping);
        } else {
            UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshHandler::Play - Animation not found: %s"),
                   *AnimParams.AnimationName.ToString());
        }
    }

    UE_LOG(LogTemp, Log, TEXT("SkeletalMeshHandler::Play - Playing animation for entity %d"),
           TsEntity->ID);
}

void SkeletalMeshHandler::Stop(Entity* TsEntity, UObject* Target) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    USkeletalMeshComponent* MeshComponent = GetMeshComponent(Actor);
    if (!MeshComponent) return;

    MeshComponent->StopAnimation();

    UE_LOG(LogTemp, Log, TEXT("SkeletalMeshHandler::Stop - Stopped animation for entity %d"),
           TsEntity->ID);
}

void SkeletalMeshHandler::ApplyProperties(UObject* Target,
                                   const std::map<std::string, std::string>& Properties) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    USkeletalMeshComponent* MeshComponent = Actor->FindComponentByClass<USkeletalMeshComponent>();
    if (!MeshComponent) return;

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
