#pragma once
#include "../HAL/InputDevices.hxx"
#include <string>
#include <json11.hpp>

class ScreenManager;

enum screen_color
{
    SCREEN_COLOR_BLACK = 0x000000,
    SCREEN_COLOR_WHITE = 0xFFFFFF,
    SCREEN_COLOR_RED   = 0xFF0000,
    SCREEN_COLOR_GREEN = 0x00FF00,
    SCREEN_COLOR_BLUE  = 0x0000FF
};

class ScreenBase
{
public:
    ScreenBase(ScreenManager* screenManager, const json11::Json &jsonConfig)
    : screenManager_(screenManager)
    {
        std::vector<std::string> attribNames =
            { "name", "id", "nextScreenId","integrationId" };
        // copy attribute value from json, and ensure there is entry in the attribs_
        for(const auto & attribName :attribNames)
            attribs_[attribName] = (
                ! jsonConfig[attribName].is_null()
                ? jsonConfig[attribName].string_value()
                : ""
            );

        // if no json "id" attribute is provided, use our "name" attribute
        if(jsonConfig["id"].is_null())
            SetId(GetName());
    }
    virtual ~ScreenBase() = default;

    virtual void Render() = 0;
    virtual void handle_input_event(const InputDeviceType device_type, const struct input_event& event) = 0;

    // These getters are safe, because the ctor ensures that there is an entry in attribs_
    inline const std::string &GetId() const { return attribs_["id"]; }
    inline void SetId(std::string const &id) { attribs_["id"] = id; }

	inline void SetName(std::string const &str) { attribs_["name"] = str; }
	inline const std::string &GetName() const { return attribs_["name"]; }

    inline const std::string &GetIntegrationId() const { return attribs_["integrationId"]; }
    inline void SetIntegrationId(std::string const &id) { attribs_["integrationId"] = id; }

    inline const std::string &GetNextScreenId() const { return attribs_["nextScreenId"]; }
    void SetNextScreenId(std::string const &id) { attribs_["nextScreenId"] = id; }

protected:
    ScreenManager* screenManager_ = nullptr;
private:
    mutable std::map<std::string, std::string> attribs_;
};