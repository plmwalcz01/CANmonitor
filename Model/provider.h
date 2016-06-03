#pragma once
#include <memory>

namespace CANMonitor{

    class settings;
    class provider
    {
    public:
    static settings& GetSettingsAsSingleton();
    private:
        static std::unique_ptr<settings> mSettingsInstance;
        explicit provider(const provider& rhs) = delete;
        provider& operator= (const provider& rhs) = delete;
    };
}



