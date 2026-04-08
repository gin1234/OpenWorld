#include "WidgetHandler.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "../StaticResourceHandler.h"
#include "../ResourceManager.h"
#include "../PropertyParser.h"

namespace TSEngine {

static TStaticResourceHandler<WidgetHandler> _Register("Widget");

WidgetHandler::WidgetHandler() = default;

void WidgetHandler::LoadAsync(const FSoftObjectPath& Path,
                               FResourceLoadedCallback Callback) {
    PendingLoads[Path] = Callback;

    FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
        this, &WidgetHandler::OnWidgetLoaded, Path, Callback);

    ResourceManager::GetInstance()->GetStreamableManager()->RequestAsyncLoad(
        Path, Delegate);
}

void WidgetHandler::OnWidgetLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback) {
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

UUserWidget* WidgetHandler::LoadWidget(const FSoftObjectPath& Path) {
    UObject* LoadedObject = Path.ResolveObject();
    return Cast<UUserWidget>(LoadedObject);
}

bool WidgetHandler::Update(Entity* TsEntity, UObject* Target,
                           std::map<std::string, std::string> Property) {
    UUserWidget* Widget = Cast<UUserWidget>(Target);
    if (!Widget) {
        UCanvasPanel* Canvas = Cast<UCanvasPanel>(Target);
        if (!Canvas) {
            UE_LOG(LogTemp, Error, TEXT("WidgetHandler: Target is not UUserWidget or UCanvasPanel"));
            return false;
        }
        ApplyCanvasProperties(Canvas, Property);
        return true;
    }

    ApplyWidgetProperties(Widget, Property);
    return true;
}

void WidgetHandler::ApplyWidgetProperties(UUserWidget* Widget,
                                           const std::map<std::string, std::string>& Properties) {
    if (!Widget) return;

    for (const auto& [Key, Value] : Properties) {
        if (Key == "Visibility") {
            Widget->SetVisibility(PropertyParser::ParseVisibility(Value));
        } else if (Key == "RenderOpacity") {
            Widget->SetRenderOpacity(std::stof(Value));
        }
    }
}

void WidgetHandler::ApplyCanvasProperties(UCanvasPanel* Canvas,
                                          const std::map<std::string, std::string>& Properties) {
    if (!Canvas) return;

    for (const auto& [Key, Value] : Properties) {
        if (Key == "Visibility") {
            Canvas->SetVisibility(PropertyParser::ParseVisibility(Value));
        } else if (Key == "RenderOpacity") {
            Canvas->SetRenderOpacity(std::stof(Value));
        }
    }
}

void WidgetHandler::ApplyProperties(UObject* Target,
                                     const std::map<std::string, std::string>& Properties) {
    UUserWidget* Widget = Cast<UUserWidget>(Target);
    if (Widget) {
        ApplyWidgetProperties(Widget, Properties);
        return;
    }

    UCanvasPanel* Canvas = Cast<UCanvasPanel>(Target);
    if (Canvas) {
        ApplyCanvasProperties(Canvas, Properties);
    }
}

} // namespace TSEngine
