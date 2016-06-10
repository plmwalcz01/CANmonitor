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
        WireDisplayUpdate();

        mSetupTab.setBaudRate(config.getBaudRate());
        mSetupTab.setSamplingPoint(config.getSamplingPoint());
        mSetupTab.setFoundDevices(mDevice.getFoundDevices());
        mSetupTab.setCommands(config.getCommandsAsModel());
    }
    SetuptabViewManager::~SetuptabViewManager()
    {
        Utils::DestructorMsg(this);
    }

    void SetuptabViewManager::onConnected()
    {
        emit NotifyConnectedEnabled(false);
        emit NotifyDisconnectedEnabled(true);
        emit NotifyMessagesEnabled(true);
        emit NotifyControlTabEnabled(true);
    }

    void SetuptabViewManager::onDisconnected()
    {
        emit NotifyDisconnectedEnabled(false);
        emit NotifyConnectedEnabled(true);
        emit NotifyMessagesEnabled(false);
        emit NotifyControlTabEnabled(false);
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

        connect(&mDevice, &Device::NotifyConnected , this, &SetuptabViewManager::onConnected);
        connect(&mDevice, &Device::NotifyDisonnected , this, &SetuptabViewManager::onDisconnected);

        connect(&mSetupTab, &SetupTab::NotifyDisconnectedClicked, &mDevice, &Device::Disconnect);
        connect(&mDevice, &Device::NotifyDisonnected , &mSetupTab, &SetupTab::onDisonnected);
        connect(&mSetupTab, &SetupTab::NotifySendClicked, &mDevice, &Device::onSendRequest);
        connect(&mSetupTab, &SetupTab::NotifyReciveClicked, &mDevice, &Device::onReciveRequest);
        connect(&mDevice, &Device::NotifyDataRead , &mSetupTab, &SetupTab::onDataRecived);
        connect(&mDevice, &Device::NotifyDataSend , &mSetupTab, &SetupTab::onDataSend);
    }

    void SetuptabViewManager::WireDisplayUpdate()
    {
        connect(this, &SetuptabViewManager::NotifyConnectedEnabled , &mSetupTab, &SetupTab::onConnectedEnabled);
        connect(this, &SetuptabViewManager::NotifyDisconnectedEnabled , &mSetupTab, &SetupTab::onDisconnectedEnabled);
        connect(this, &SetuptabViewManager::NotifyControlTabEnabled , &mSetupTab, &SetupTab::onControlTabEnabled);
        connect(this, &SetuptabViewManager::NotifyMessagesEnabled , &mSetupTab, &SetupTab::onMessagesEnabled);
    }
}

