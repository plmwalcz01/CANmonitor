#include "device.h"
#include <Model/cansocket.h>
#include "utils.h"
#include <QDebug>
#include <QStringListModel>

namespace CANMonitor
{
    Device::Device(QObject *parent, CANSocket &CANSock) :
        QObject(parent), mCanSocket(CANSock)
    {
        WireConnections();
    }
    Device::~Device()
    {
        Utils::DestructorMsg(this);
    }

    bool Device::Connect(const QString &device)
    {
        mCanSocket.Connect(device);
    }

    void Device::Disconnect()
    {
        mCanSocket.Disconnect();
    }

    QStringListModel& Device::getFoundDevices()
    {
        return mCanSocket.getFoundDevices();
    }
    void Device::WireConnections()
    {
        connect(&mCanSocket, &CANSocket::NotifyConnected, this, &Device::onConnected);
        connect(&mCanSocket, &CANSocket::NotifyDisconnected, this, &Device::onDisonnected);
        connect(&mCanSocket, &CANSocket::NotifyReadData, this, &Device::onRecivedData);
        connect(&mCanSocket, &CANSocket::NotifyDevicesFound, this, &Device::onDevicesFound);
    }

    void Device::onConnected()
    {
        emit NotifyConnected();
    }
    void Device::onDisonnected()
    {
        emit NotifyDisonnected();
    }
    void Device::onRecivedData(const QString &readData)
    {
        emit NotifyDataRead(readData);
    }

    void Device::onReciveRequest()
    {
        mCanSocket.readyRead();
    }

    void Device::onBaudRateCahnge()
    {
        //TODO do sth with new baudrate
    }

    void Device::onSamplingPointChange()
    {
        //TODO do sth with new samplingpoint
    }

    void Device::onSendRequest(const QString &dataToSend)
    {
        //TODO send some data
        emit NotifyDataSend(dataToSend);
    }
    void Device::onDevicesFound()
    {
        emit NotifyDevicesFound(getFoundDevices());
    }

    void Device::onRefreshDevices()
    {
        mCanSocket.checkAvailableUSBPorts();
    }
}
