#pragma once
#include "IResourceHandler.h"
#include "Engine/StreamableManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

namespace TSEngine {

class WidgetHandler : public IUpdatableResourceHandler {
public:
    WidgetHandler();

    virtual std::string GetType() const override { return "Widget"; }

    virtual void LoadAsync(const FSoftObjectPath& Path,
                          FResourceLoadedCallback Callback) override;

    virtual bool Update(Entity* TsEntity, UObject* Target,
                       std::map<std::string, std::string> Property) override;

    virtual void ApplyProperties(UObject* Target,
                                const std::map<std::string, std::string>& Properties) override;

    UUserWidget* LoadWidget(const FSoftObjectPath& Path);

private:
    void OnWidgetLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback);

    std::unordered_map<FSoftObjectPath, FResourceLoadedCallback> PendingLoads;
};

} // namespace TSEngine
