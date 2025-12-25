#pragma once

#include "ScreenBase.hpp"
#include "../ScreenManager.hpp"
#include "../HAL/HAL.hpp"
#include "../Backplate/BackplateComms.hpp"

class HomeScreen : public ScreenBase
{
public:
    HomeScreen(
        HAL *hal
        , ScreenManager *screenManager
        , const json11::Json &jsonConfig
        , BackplateComms *backplateComms
    )
        : ScreenBase(jsonConfig)
        , screenManager_(screenManager)
        , display_(hal->display)
        , beeper_(hal->beeper)
        , backplateComms_(backplateComms)
	{
        if(GetName() == "")
            SetName("Home");
	}

    virtual ~HomeScreen() = default;

    void Render() override;
    std::string TimeToString(time_t time);
    std::string GetTemperatureString();
    std::string GetHumidityString();
    void handle_input_event(const InputDeviceType device_type, const struct input_event &event) override;

private:
    int currentColorIndex = 0;
    IDisplay *display_;
    Beeper *beeper_;
    ScreenManager *screenManager_;
    BackplateComms *backplateComms_;
};
