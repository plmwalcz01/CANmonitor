#include "setubtabviewmanager.h"
#include "utils.h"
#include "View/setuptab.h"
#include "Model/device.h"
#include "Model/settings.h"
#include <iostream>
#include <QDebug>




namespace CANMonitor
{
    SetuptabViewManager::SetuptabViewManager(QObject *parent, SetupTab &tab, Device &device, settings& config) :
        QObject(parent), mSetupTab(tab), mDevice(device)
    {
        WireSettings(config);
        config.ParseJsonData();
        WireBaudRateAndSamplingPoint();
        WireButtons();

        mSetupTab.setBaudRate(config.getBaudRate());
        mSetupTab.setSamplingPoint(config.getSamplingPoint());
        mSetupTab.setFoundDevices(mDevice.getFoundDevices());
        mSetupTab.setCommands(config.getCommandsAsModel());
    }
    SetuptabViewManager::~SetuptabViewManager()
    {
        Utils::DestructorMsg(this);
    }
    void SetuptabViewManager::WireSettings(settings& config)
    {
        connect(&config, &settings::NotifyStatusMessage, &mSetupTab, &SetupTab::onStatusUpdated);
    }
    void SetuptabViewManager::WireBaudRateAndSamplingPoint()
    {
        connect(&mSetupTab, &SetupTab::NotifyBaudRateChanged, &mDevice, &Device::onBaudRateCahnge);
        connect(&mSetupTab, &SetupTab::NotifySamplingPointChanged, &mDevice, &Device::onSamplingPointChange);
    }

    void SetuptabViewManager::WireButtons()
    {
        connect(&mSetupTab, &SetupTab::NotifyRefreshDevices, &mDevice, &Device::onRefreshDevices);
        connect(&mDevice, &Device::NotifyDevicesFound, &mSetupTab, &SetupTab::onFoundDevices);
        connect(&mSetupTab, &SetupTab::NotifyConnectedClicked, &mDevice, &Device::Connect);
        connect(&mDevice, &Device::NotifyConnected , &mSetupTab, &SetupTab::onConnected);
        connect(&mSetupTab, &SetupTab::NotifyDisconnectedClicked, &mDevice, &Device::Disconnect);
        connect(&mDevice, &Device::NotifyDisonnected , &mSetupTab, &SetupTab::onDisonnected);
        connect(&mSetupTab, &SetupTab::NotifySendClicked, &mDevice, &Device::onSendRequest);
        connect(&mSetupTab, &SetupTab::NotifyReciveClicked, &mDevice, &Device::onReciveRequest);
        connect(&mDevice, &Device::NotifyDataRead , &mSetupTab, &SetupTab::onDataRecived);
        connect(&mDevice, &Device::NotifyDataSend , &mSetupTab, &SetupTab::onDataSend);
    }
}

