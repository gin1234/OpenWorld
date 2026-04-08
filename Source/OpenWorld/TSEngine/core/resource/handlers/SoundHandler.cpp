#include "SoundHandler.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../StaticResourceHandler.h"
#include "../ResourceManager.h"
#include "../PropertyParser.h"

namespace TSEngine {

static TStaticPlayableResourceHandler<SoundHandler> _RegisterPlayable("Sound");

SoundHandler::SoundHandler() = default;

void SoundHandler::LoadAsync(const FSoftObjectPath& Path,
                             FResourceLoadedCallback Callback) {
    PendingLoads[Path] = Callback;

    FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
        this, &SoundHandler::OnSoundLoaded, Path, Callback);

    ResourceManager::GetInstance()->GetStreamableManager()->RequestAsyncLoad(
        Path, Delegate);
}

void SoundHandler::OnSoundLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback) {
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

AActor* SoundHandler::CreateActor(Entity* TsEntity, UObject* LoadedSound,
                                const std::map<std::string, std::string>& Properties) {
    USoundBase* Sound = Cast<USoundBase>(LoadedSound);
    if (!Sound) {
        UE_LOG(LogTemp, Error, TEXT("SoundHandler: Loaded object is not a USoundBase"));
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
        UE_LOG(LogTemp, Error, TEXT("SoundHandler: Could not find valid World"));
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = *FString::Printf(TEXT("Sound_%d"), TsEntity->ID);
    SpawnParams.Owner = nullptr;
    SpawnParams.Instigator = nullptr;

    FVector Location = FVector::ZeroVector;

    auto it = Properties.find("Location");
    if (it != Properties.end()) {
        Location = PropertyParser::ParseVector(it->second);
    }

    AActor* Actor = World->SpawnActor<AActor>(AActor::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);

    if (!Actor) {
        UE_LOG(LogTemp, Error, TEXT("SoundHandler: Failed to spawn AActor"));
        return nullptr;
    }

    USceneComponent* SceneComponent = NewObject<USceneComponent>(Actor);
    SceneComponent->RegisterComponent();
    Actor->SetRootComponent(SceneComponent);

    UAudioComponent* AudioComponent = NewObject<UAudioComponent>(Actor);
    AudioComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
    AudioComponent->SetSound(Sound);
    AudioComponent->RegisterComponent();

    ApplyProperties(Actor, Properties);

    UE_LOG(LogTemp, Log, TEXT("SoundHandler: Successfully created actor %s for entity %d"),
           *Actor->GetName(), TsEntity->ID);

    return Actor;
}

UAudioComponent* SoundHandler::GetAudioComponent(AActor* Actor) {
    if (!Actor) return nullptr;
    return Actor->FindComponentByClass<UAudioComponent>();
}

void SoundHandler::Play(Entity* TsEntity, UObject* Target,
                     const std::map<std::string, std::string>& Params) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    UAudioComponent* AudioComp = GetAudioComponent(Actor);
    if (!AudioComp) {
        UE_LOG(LogTemp, Error, TEXT("SoundHandler::Play - No AudioComponent"));
        return;
    }

    FSoundParams SoundParams = PropertyParser::ParseSoundParams(Params);

    AudioComp->SetVolumeMultiplier(SoundParams.VolumeMultiplier);
    AudioComp->SetPitchMultiplier(SoundParams.PitchMultiplier);

    AudioComp->Play(SoundParams.StartTime);

    UE_LOG(LogTemp, Log, TEXT("SoundHandler::Play - Playing sound for entity %d"),
           TsEntity->ID);
}

void SoundHandler::Stop(Entity* TsEntity, UObject* Target) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    UAudioComponent* AudioComp = GetAudioComponent(Actor);
    if (!AudioComp) return;

    AudioComp->Stop();

    UE_LOG(LogTemp, Log, TEXT("SoundHandler::Stop - Stopped sound for entity %d"),
           TsEntity->ID);
}

void SoundHandler::Pause(Entity* TsEntity, UObject* Target) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    UAudioComponent* AudioComp = GetAudioComponent(Actor);
    if (!AudioComp) return;

    AudioComp->Pause();

    UE_LOG(LogTemp, Log, TEXT("SoundHandler::Pause - Paused sound for entity %d"),
           TsEntity->ID);
}

void SoundHandler::ApplyProperties(UObject* Target,
                                const std::map<std::string, std::string>& Properties) {
    AActor* Actor = Cast<AActor>(Target);
    if (!Actor) return;

    USceneComponent* Root = Actor->GetRootComponent();
    if (!Root) return;

    for (const auto& [Key, Value] : Properties) {
        if (Key == "Location") {
            Root->SetRelativeLocation(PropertyParser::ParseVector(Value));
        }
    }
}

} // namespace TSEngine
