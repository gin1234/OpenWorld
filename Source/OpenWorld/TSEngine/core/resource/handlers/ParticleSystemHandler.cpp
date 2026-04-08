#include "ParticleSystemHandler.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "../StaticResourceHandler.h"
#include "../ResourceManager.h"
#include "../PropertyParser.h"

namespace TSEngine {

static TStaticResourceHandler<ParticleSystemHandler> _RegisterPlayable("ParticleSystem");

ParticleSystemHandler::ParticleSystemHandler() = default;

void ParticleSystemHandler::LoadAsync(const FSoftObjectPath& Path,
                              FResourceLoadedCallback Callback) {
    PendingLoads[Path] = Callback;

    FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
        this, &ParticleSystemHandler::OnParticleLoaded, Path, Callback);

    ResourceManager::GetInstance()->GetStreamableManager()->RequestAsyncLoad(
        Path, Delegate);
}

void ParticleSystemHandler::OnParticleLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback) {
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

AActor* ParticleSystemHandler::CreateActor(Entity* TsEntity, UObject* LoadedPS,
                                   const std::map<std::string, std::string>& Properties) {
    UParticleSystem* ParticleSystem = Cast<UParticleSystem>(LoadedPS);
    if (!ParticleSystem) {
        UE_LOG(LogTemp, Error, TEXT("ParticleSystemHandler: Loaded object is not a UParticleSystem"));
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
        UE_LOG(LogTemp, Error, TEXT("ParticleSystemHandler: Could not find valid World"));
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = *FString::Printf(TEXT("ParticleSystem_%d"), TsEntity->ID);
    SpawnParams.Owner = nullptr;
    SpawnParams.Instigator = nullptr;

    FVector Location = FVector::ZeroVector;
    FRotator Rotation = FRotator::ZeroRotator;

    auto it = Properties.find("Location");
    if (it != Properties.end()) {
        Location = PropertyParser::ParseVector(it->second);
    }
    it = Properties.find("Rotation");
    if (it != Properties.end()) {
        Rotation = PropertyParser::ParseRotator(it->second);
    }

    AActor* Actor = World->SpawnActor<AActor>(AActor::StaticClass(), Location, Rotation, SpawnParams);

    if (!Actor) {
        UE_LOG(LogTemp, Error, TEXT("ParticleSystemHandler: Failed to spawn AActor"));
        return nullptr;
    }

    USceneComponent* SceneComponent = NewObject<USceneComponent>(Actor);
    SceneComponent->RegisterComponent();
    Actor->SetRootComponent(SceneComponent);

    UParticleSystemComponent* ParticleComponent = NewObject<UParticleSystemComponent>(Actor);
    ParticleComponent->bAutoAttach = true;
    ParticleComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
    ParticleComponent->SetTemplate(ParticleSystem);
    ParticleComponent->RegisterComponent();

    ApplyProperties(Actor, Properties);

    UE_LOG(LogTemp, Log, TEXT("ParticleSystemHandler: Successfully created actor %s for entity %d"),
           *Actor->GetName(), TsEntity->ID);

    return Actor;
}

UParticleSystemComponent* ParticleSystemHandler::GetParticleComponent(AActor* Actor) {
    if (!Actor) return nullptr;
    return Actor->FindComponentByClass<UParticleSystemComponent>();
}

void ParticleSystemHandler::Play(Entity* TsEntity, UObject* Target,
                         const std::map<std::string, std::string>& Params) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    UParticleSystemComponent* ParticleComp = GetParticleComponent(Actor);
    if (!ParticleComp) {
        UE_LOG(LogTemp, Error, TEXT("ParticleSystemHandler::Play - No ParticleSystemComponent"));
        return;
    }

    ParticleComp->Activate(true);

    UE_LOG(LogTemp, Log, TEXT("ParticleSystemHandler::Play - Activated particle for entity %d"),
           TsEntity->ID);
}

void ParticleSystemHandler::Stop(Entity* TsEntity, UObject* Target) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    UParticleSystemComponent* ParticleComp = GetParticleComponent(Actor);
    if (!ParticleComp) return;

    ParticleComp->Deactivate();

    UE_LOG(LogTemp, Log, TEXT("ParticleSystemHandler::Stop - Stopped particle for entity %d"),
           TsEntity->ID);
}

void ParticleSystemHandler::ApplyProperties(UObject* Target,
                                    const std::map<std::string, std::string>& Properties) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    USceneComponent* Root = Actor->GetRootComponent();
    if (!Root) return;

    for (const auto& [Key, Value] : Properties) {
        if (Key == "Location") {
            Root->SetRelativeLocation(PropertyParser::ParseVector(Value));
        } else if (Key == "Rotation") {
            Root->SetRelativeRotation(PropertyParser::ParseRotator(Value));
        } else if (Key == "Scale") {
            Root->SetRelativeScale3D(PropertyParser::ParseVector(Value));
        }
    }
}

} // namespace TSEngine
