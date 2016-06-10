#pragma once
#include <QObject>

namespace CANMonitor{

    class MainView;
    class SetupTab;
    class Device;
    class SetuptabViewManager;
    class ControlTab;
    class ControlTabViewManager;

    class Startup final : public QObject
    {
        Q_OBJECT
    public:
        explicit Startup();
        void show() const;
        ~Startup();
    private:
        SetupTab& mSetupTab;
        ControlTab& mControlTab;
        MainView& mMainView;
        Device* mDevicePtr;    
        SetuptabViewManager* mSetupViewMgr;
        ControlTabViewManager* mControlTabMgr;
        explicit Startup(const Startup& rhs) = delete;
        Startup& operator= (const Startup& rhs) = delete;
    };
}
