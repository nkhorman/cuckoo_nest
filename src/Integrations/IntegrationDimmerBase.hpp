#pragma once
#include <string>

class IntegrationDimmerBase 
{
    public:
        virtual ~IntegrationDimmerBase() = default;

        virtual SwitchState GetState() = 0;
        virtual void TurnOn() = 0;
        virtual void TurnOff() = 0;

        virtual void SetBrightness(int brightness) = 0;
        virtual int GetBrightness() = 0;

        inline std::string const &GetId() const { return id; }
        inline void SetId(std::string const &newId) { id = newId; }

        inline std::string GetName() const { return name; }
        inline void SetName(const std::string& newName) { name = newName; }

    private:
        std::string id;
        std::string name;
};
