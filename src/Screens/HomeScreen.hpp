#pragma once

#include "ScreenBase.hpp"
#include "../ScreenManager.hpp"

class HomeScreen : public ScreenBase
{
public:
    HomeScreen(
        ScreenManager *screenManager
        , const json11::Json &jsonConfig
    )
        : ScreenBase(screenManager, jsonConfig)
	{
        if(GetName() == "")
            SetName("Home");
        beeper_ = screenManager_->HalBeeper();
        display_ = screenManager_->HalDisplay();
        backplateComms_ = screenManager_->GetBackplaceComms();
	}

    virtual ~HomeScreen() = default;

    void Render() override;
    std::string TimeToString(time_t time);
    std::string GetTemperatureString();
    std::string GetHumidityString();
    void handle_input_event(const InputDeviceType device_type, const struct input_event &event) override;

private:
    int currentColorIndex = 0;
    Beeper* beeper_ = nullptr;
    IDisplay* display_ = nullptr;
    BackplateComms *backplateComms_ = nullptr;
};
