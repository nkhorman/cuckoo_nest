#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <map>
#include <cstddef>
#include <json11.hpp>
#include "HAL/HAL.hpp"
#include "Screens/ScreenBase.hpp"
#include "Screens/MenuIcon.hpp"
#include "Integrations/IntegrationContainer.hpp"
#include "Backplate/BackplateComms.hpp"
#include "logger.h"

class ScreenManager 
{
public:
    ScreenManager(
        HAL *hal
        , IntegrationContainer* integrationContainer
        , BackplateComms *backplateComms
    )
        : screen_history_()
        , current_screen_(nullptr)
        , hal_(hal)
        , integrationContainer_(integrationContainer)
        , backplateComms_(backplateComms)
    {}

    virtual ~ScreenManager() {};
    
    void GoToNextScreen(std::string const &id);
    void GoToPreviousScreen();
    void RenderCurrentScreen();
    void ProcessInputEvent(const InputDeviceType device_type, const input_event &event);

    void LoadScreensFromConfig(const std::string &config_path);

    ScreenBase* GetScreenById(std::string const &id) const
    {
        auto it = screens_.find(id);
        return (it != screens_.end()) ? it->second.get() : nullptr;
    }

    inline size_t CountScreens() const { return screens_.size(); }    
    inline void AddScreen(std::unique_ptr<ScreenBase> screen) { screens_[screen->GetId()] = std::move(screen); }
    inline IntegrationContainer* GetIntegrationContainer() const { return integrationContainer_; }

private:
    std::string ReadFileContents(const std::string &filepath) const;

    void BuildMenuScreenFromJSON(const json11::Json &screenJson);

    std::stack<ScreenBase*> screen_history_;
    ScreenBase* current_screen_;
    std::map<std::string, std::unique_ptr<ScreenBase>> screens_;
    HAL *hal_;
    IntegrationContainer* integrationContainer_;
    BackplateComms *backplateComms_;
};