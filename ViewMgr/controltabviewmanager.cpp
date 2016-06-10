#include "controltabviewmanager.h"
#include "utils.h"
#include "Model/device.h"
#include "View/controltab.h"
#include "Model/settings.h"
namespace CANMonitor
{
    ControlTabViewManager::ControlTabViewManager(QObject *parent, ControlTab &controlTab, Device &device, settings &config) :
        QObject(parent), mControlTab(controlTab), mDevice(device), mConfig(config)
    {
        WireControls();
        WireConnected();
    }

    ControlTabViewManager::~ControlTabViewManager()
    {
        Utils::DestructorMsg(this);
    }

    void ControlTabViewManager::onConnected()
    {
        emit NotifyControlTabEnabled(true);
    }

    void ControlTabViewManager::onDisconnected()
    {
        emit NotifyControlTabEnabled(false);
    }

    void ControlTabViewManager::WireControls()
    {
        connect(&mControlTab, &ControlTab::NotifyPWChange, &mDevice, &Device::onPWChange);
    }

    void ControlTabViewManager::WireConnected()
    {
        connect(&mDevice, &Device::NotifyConnected, this, &ControlTabViewManager::onConnected );
        connect(&mDevice, &Device::NotifyDisonnected, this, &ControlTabViewManager::onDisconnected );
        connect(this, &ControlTabViewManager::NotifyControlTabEnabled, &mControlTab, &ControlTab::onControlTabEnabled);
    }
}
