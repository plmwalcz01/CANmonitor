#include "provider.h"
#include "utils.h"
#include <QString>
#include"Model/settings.h"

namespace CANMonitor
{
    static QString SETTINGS_FILE = QStringLiteral("settings.json");
    std::unique_ptr<settings> provider::mSettingsInstance = nullptr;
    settings& provider::GetSettingsAsSingleton()
    {
        if(mSettingsInstance == nullptr)
        {
            mSettingsInstance = Utils::make_unique<settings>(nullptr, SETTINGS_FILE);
        }
        return *mSettingsInstance;
    }
}
