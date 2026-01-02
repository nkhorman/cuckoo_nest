#pragma once

#include "IntegrationSwitchBase.hpp"
#include "HomeAssistantCreds.hpp"
#include "CurlWrapper.hpp"
#include "logger.h"
#include <json11.hpp>

class HomeAssistantSwitch : public IntegrationSwitchBase
{
public:
    HomeAssistantSwitch(const HomeAssistantCreds &creds, const std::string &entity_id)
        : creds_(creds)
        , entityId_(entity_id)
    {}

    virtual ~HomeAssistantSwitch() = default;

    const std::string &GetEntityId() const { return entityId_; }

    SwitchState GetState() override
    {
        // Implementation to get state from Home Assistant
        json11::Json status = statusQuery(GetEntityId());
        SwitchState ss =
            (
                status.is_object()
                    && !status["state"].is_null()
                    && status["state"].is_string()
                    && status["state"].string_value() == "on"
                ? SwitchState::ON
                : SwitchState::OFF
            );

        return ss;
    }

    void TurnOn() override
    {
        // Implementation to turn on the switch via Home Assistant
        execute("switch/turn_on");
    }

    void TurnOff() override
    {
        // Implementation to turn off the switch via Home Assistant
        execute("switch/turn_off");
    }

private:
    HomeAssistantCreds creds_;
    std::string entityId_;
    static std::size_t callback( const char* in, std::size_t size, std::size_t num, std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    };
    std::string httpData;

    json11::Json statusQuery(std::string id)
    {
        json11::Json rc;

        static CurlWrapper curl_wrapper2;
        static bool curl_initialized2 = false;

        if(!curl_initialized2)
        {
            curl_initialized2 = curl_wrapper2.initialize();
            if (!curl_initialized2)
                LOG_ERROR_STREAM("Failed to initialize libcurl");
        }

        if(curl_initialized2)
        {
            // Prepare headers
            struct curl_slist *headers = nullptr;
            std::string authHeader = "Authorization: Bearer " + creds_.GetToken();
            headers = curl_wrapper2.slist_append(headers, authHeader.c_str());
            headers = curl_wrapper2.slist_append(headers, "Content-Type: application/json");

            CURL *curl = curl_wrapper2.easy_init();
            if (curl)
            {
                std::string url = creds_.GetUrl() + "/api/states/" + id;

                LOG_INFO_STREAM("HomeAssistantSwitch: URL: " << url);
                curl_wrapper2.easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_wrapper2.easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_wrapper2.easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
                curl_wrapper2.easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);
                CURLcode res = curl_wrapper2.easy_perform(curl);

                if(res == CURLE_OK)
                {
                    LOG_INFO_STREAM("HomeAssistantSwitch: Status call completed = "<< httpData);
                    std::string parse_error;
                    rc = json11::Json::parse(httpData, parse_error);
                    if (!parse_error.empty())
                        LOG_ERROR_STREAM("HomeAssistantSwitch: Status JSON parse error: " << parse_error);
                }
                else
                    LOG_ERROR_STREAM("HomeAssistantSwitch Status call failed: " << curl_wrapper2.easy_strerror(res));

                curl_wrapper2.easy_cleanup(curl);
                curl_wrapper2.slist_free_all(headers);
            }
        }

        return rc;
    }

    void execute(std::string action)
    {
        // Implementation to call Home Assistant service
        LOG_INFO_STREAM("Calling Home Assistant service: " << creds_.GetUrl());

        static CurlWrapper curl_wrapper;
        static bool curl_initialized = false;

        if (!curl_initialized)
        {
            curl_initialized = curl_wrapper.initialize();
            if (!curl_initialized)
            {
                LOG_ERROR_STREAM("Failed to initialize libcurl - functionality disabled");
                return;
            }
        }

        // Prepare JSON data
        std::string jsonData = "{\"entity_id\": \"" + entityId_ + "\"}";

        // Prepare headers
        struct curl_slist *headers = nullptr;
        std::string authHeader = "Authorization: Bearer " + creds_.GetToken();
        headers = curl_wrapper.slist_append(headers, authHeader.c_str());
        headers = curl_wrapper.slist_append(headers, "Content-Type: application/json");

        CURL *curl = curl_wrapper.easy_init();
        if (curl)
        {
            std::string url = creds_.GetUrl() + "/api/services/" + action;
            LOG_INFO_STREAM("HomeAssistantSwitch: URL: " << url);
            curl_wrapper.easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_wrapper.easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_wrapper.easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

            CURLcode res = curl_wrapper.easy_perform(curl);
            if (res != CURLE_OK)
            {
                LOG_ERROR_STREAM("curl_easy_perform() failed: " << curl_wrapper.easy_strerror(res));
            }

            curl_wrapper.easy_cleanup(curl);
            curl_wrapper.slist_free_all(headers);
            LOG_INFO_STREAM("HomeAssistantSwitch: Service call completed");
        }
    }
};