#include "device.h"
#include "utils.h"
#include <QDebug>
#include <QStringListModel>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace CANMonitor
{
    Device::Device(QObject *parent) :
        QObject(parent)
    {
        WireConnections();
        mCanSocket.moveToThread( &canSocketThread );
        canSocketThread.start();
    }
    Device::~Device()
    {
        canSocketThread.exit();
        while(!canSocketThread.isFinished()){}

        Utils::DestructorMsg(this);
    }

    bool Device::Connect(const QString &device)
    {
        mCanSocket.Connect(device);
        return true;
    }

    void Device::Disconnect()
    {
        mCanSocket.Disconnect();
        emit NotifyDisonnected();
    }

    QString Device::translateCanFrame(QByteArray frame)
    {
        canBaseFrameFormat canFrame;
        memset (&canFrame, 0, sizeof(canFrame));
        //u_int8_t index = 0; can be used to keep track what part of frame is currently parsed/translated
        canFrame.IDE = ((*frame.data() >> 5) & 0x01);
        canFrame.DLC = ((*frame.data() ) & 0x0F);
        if(!canFrame.IDE)
        {
            canFrame.canID = static_cast<u_int8_t>((*(frame.data()+1)));
            canFrame.canID = canFrame.canID << 3;
            canFrame.canID |= (((*(frame.data()+2))>>5) & 0x07);
            for(u_int8_t i=0; i < canFrame.DLC; i++)
            {
                if(i < sizeof(canFrame.data))//max data lenth
                {
                    canFrame.data[i] = static_cast<u_int8_t>(*(frame.data() + 3 + i));//would look better with index variable
                }
            }
        }
        else
        {
            canFrame.canID = static_cast<u_int8_t>(*(frame.data()+1));
            canFrame.canID = canFrame.canID << 21;
            canFrame.canID |= (( (( (*(frame.data()+2)) & 0xe0) >> 5) | ((*(frame.data()+2)) & 0x03)) << 16);
            canFrame.canID |= (*(frame.data()+3)) << 8 ;
            canFrame.canID |= (*(frame.data()+4));
            for(u_int8_t i=0; i < canFrame.DLC; i++)
            {
                if(i < sizeof(canFrame.data))//max data lenth
                {
                    canFrame.data[i] = static_cast<u_int8_t>(*(frame.data() + 5 + i));//would look better with index variable
                }
            }
        }
        return canFrameToQString(canFrame);
    }
    QString Device::canFrameToQString(const canBaseFrameFormat &canFrame)
    {
        std::stringstream ss;
        ss << "frame ID: "<<std::setfill('0') << std::setw(8) << std::hex << static_cast<int>(canFrame.canID) << " ";
        if (!canFrame.IDE)
        {
            ss << "      ";
        }
        else
        {
            ss << " ext  ";
        }
        ss << "  len: " <<static_cast<int>( canFrame.DLC) <<"  message:  ";
        for(u_int8_t i; i < 8; i++)
        {
            if (i < canFrame.DLC )
            {
                ss  << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(canFrame.data[i]) << " " ;
            }
            else
            {
                ss << "-- ";
            }
        }
        return QString(ss.str().c_str());
    }
    QStringListModel& Device::getFoundDevices()
    {
        return mCanSocket.getFoundDevices();
    }

    void Device::onPWChange()
    {

    }

    void Device::onStatusChanged(QString msg)
    {
        emit NotifyStatusMsg(msg);
    }
    void Device::WireConnections()
    {
        connect(&mCanSocket, &CANSocket::NotifyConnected, this, &Device::onConnected);
        connect(&mCanSocket, &CANSocket::NotifyDisconnected, this, &Device::onDisonnected);
        connect(&mCanSocket, &CANSocket::NotifyReadData, this, &Device::onRecivedData);
        connect(&mCanSocket, &CANSocket::NotifyDevicesFound, this, &Device::onDevicesFound);
        connect(&mCanSocket, &CANSocket::NotifyStatusChanged, this, &Device::onStatusChanged);
        connect(&mCanSocket, &CANSocket::NotifyDataSend, this, &Device::onDataSend);
        connect(this, &Device::NotifyDataToSend, &mCanSocket, &CANSocket::onSendDataRequest);

    }

    void Device::onConnected()
    {
        emit NotifyConnected();
    }
    void Device::onDisonnected()
    {
        emit NotifyDisonnected();
    }
    void Device::onRecivedData(const QByteArray &readData)
    {
        QString translatedData = translateCanFrame(readData);
        emit NotifyDataRead(translatedData);
    }

    void Device::onReciveRequest()
    {
       emit NotifyDataRead( mCanSocket.getReadDataAsQString());
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
        qDebug() << "w dol device send:" + dataToSend;
        emit NotifyDataToSend(dataToSend);
    }

    void Device::onDataSend(const QString &dataSend)
    {
        qDebug() << "w gore device send:" + dataSend;
        emit NotifyDataSend(dataSend);
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
