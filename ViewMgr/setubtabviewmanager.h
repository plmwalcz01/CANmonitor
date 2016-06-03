#pragma once
#include <QObject>

namespace CANMonitor{

    class SetupTab;
    class Device;
    class settings;

    class SetuptabViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit SetuptabViewManager(QObject *parent, SetupTab& tab, Device& device, settings& config);
        ~SetuptabViewManager();


    private:
        void WireSettings(settings &config);
        void WireBaudRateAndSamplingPoint();
        void WireButtons();
        SetupTab &mSetupTab;
        Device &mDevice;
        explicit SetuptabViewManager(const SetuptabViewManager& rhs) = delete;
        SetuptabViewManager& operator= (const SetuptabViewManager& rhs) = delete;
    };
}



