#pragma once
#include <QObject>

namespace CANMonitor{
    class ControlTab;
    class Device;
    class settings;
    class ControlTabViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit ControlTabViewManager(QObject *parent, ControlTab& controlTab, Device& device, settings& config);
        ~ControlTabViewManager();

    public slots:
        void onConnected();
        void onDisconnected();

    signals:
        void NotifyControlTabEnabled(bool isEnabled);

    private:

        ControlTab& mControlTab;
        Device& mDevice;
        settings& mConfig;
        void WireControls();
        void WireConnected();
        explicit ControlTabViewManager(const ControlTabViewManager& rhs) = delete;
        ControlTabViewManager& operator= (const ControlTabViewManager& rhs) = delete;
    };
}



