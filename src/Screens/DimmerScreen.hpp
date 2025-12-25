#pragma once

#include "ScreenBase.hpp"
#include "../ScreenManager.hpp"
#include "../HAL/IDisplay.hpp"
#include "../HAL/Beeper.hpp"

class DimmerScreen : public ScreenBase
{
public:
    DimmerScreen(
        HAL *hal
        , ScreenManager* screenManager
        , const json11::Json &jsonConfig
    )
        : ScreenBase(jsonConfig)
        , screenManager_(screenManager)
        , display_(hal->display)
        , beeper_(hal->beeper)
	{
		dimmerValue = 50 * DIMMER_STEP;
        if(GetName() == "")
            SetName("Dimmer");
        // if no integration "id" attribute is provided, use our "name" attribute
        if(GetIntegrationId() == "")
            SetIntegrationId(GetName());
	}

    virtual ~DimmerScreen() = default;

    void Render() override;
    void handle_input_event(const InputDeviceType device_type, const struct input_event &event) override;


private:
    ScreenManager* screenManager_;
    Beeper* beeper_;
    IDisplay* display_;
    int dimmerValue;

    const int DIMMER_STEP = 50; // step size for each rotary event
    const int MAX_DIMMER_VALUE = 100; // maximum dimmer value
    const int MIN_DIMMER_VALUE = 0;   // minimum dimmer value
};