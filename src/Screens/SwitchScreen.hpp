#pragma once

#include "ScreenBase.hpp"
#include "../ScreenManager.hpp"
#include "../HAL/HAL.hpp"
#include "../Integrations/IntegrationActionBase.hpp"

class SwitchScreen : public ScreenBase
{
public:
    SwitchScreen(
        HAL* hal
        , ScreenManager* screenManager
        , const json11::Json &jsonConfig
    )
        : ScreenBase(jsonConfig)
        , screenManager_(screenManager)
        , display_(hal->display)
        , beeper_(hal->beeper)
        , rotaryAccumulator(0)
        , switchState(SwitchState::OFF)
        , selectedOption(SelectedOption::TOGGLE)
	{
        if(GetName() == "")
            SetName("Switch");
        // if no integration "id" attribute is provided, use our "name" attribute
        if(GetIntegrationId() == "")
            SetIntegrationId(GetName());
	}

    virtual ~SwitchScreen() = default;

    void Render() override;
    void handle_input_event(const InputDeviceType device_type, const struct input_event &event) override;
private:

    enum class SwitchState {
        OFF,
        ON
    }switchState;

    enum class SelectedOption {
        TOGGLE,
        BACK
    }selectedOption;

    ScreenManager* screenManager_;
    Beeper* beeper_;
    IDisplay* display_;
    int rotaryAccumulator;
};