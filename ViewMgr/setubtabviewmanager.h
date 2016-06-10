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

    signals:
        void NotifyConnectedEnabled(bool value);
        void NotifyDisconnectedEnabled(bool value);
        void NotifyMessagesEnabled(bool value);
        void NotifyControlTabEnabled(bool value);

    private slots:
        void onConnected();
        void onDisconnected();

    private:
        void WireSettings(settings &config);
        void WireBaudRateAndSamplingPoint();
        void WireButtons();
        void WireDisplayUpdate();
        SetupTab &mSetupTab;
        Device &mDevice;
        explicit SetuptabViewManager(const SetuptabViewManager& rhs) = delete;
        SetuptabViewManager& operator= (const SetuptabViewManager& rhs) = delete;
    };
}



