#include "cansocket.h"
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <QByteArray>
#include "utils.h"

namespace CANMonitor
{
    CANSocket::CANSocket(QObject *parent) :
        QObject(parent), mFoundDevices(*new QStringListModel(this))
    {
        checkAvailableUSBPorts();
        mReadBuffer.resize(64);
    }

    CANSocket::~CANSocket()
    {
        Utils::DestructorMsg(this);
    }

    QByteArray CANSocket::getReadData()
    {
        return mReadBuffer;
    }

    QString CANSocket::getReadDataAsQString()
    {
        return mReadData;
    }

    QStringListModel& CANSocket::getFoundDevices()
    {
        return mFoundDevices;
    }
    bool CANSocket::Connect(QString device)
    {
        inputFile = new QFile(device);

        if(!inputFile->open(QFile::ReadWrite))
        {
            std::cout << "Cant open!" << qPrintable(device);
            std::cout << qPrintable(inputFile->errorString()) ;
            return false;
        }

        mSNRead = new QSocketNotifier(inputFile->handle(), QSocketNotifier::Read);
        mSNWrite = new QSocketNotifier(inputFile->handle(), QSocketNotifier::Write);
        mSNRead->setEnabled(true);
        QObject::connect(mSNRead, SIGNAL(activated(int)), this , SLOT(readyRead()));
        QObject::connect(mSNWrite, SIGNAL(activated(int)), this , SLOT(readySend()));
        QObject::connect( this , SIGNAL(DidWriting(int)), mSNWrite, SIGNAL(activated(int)));
        isConnected = true;
        emit NotifyConnected();

        return true;
    }

    void CANSocket::readySend()
    {

        qDebug() << "ready send";
        mSNWrite->setEnabled(false);
        mSNRead->setEnabled(true);
    }

    void CANSocket::readyRead()
    {
        if(isConnected)
        {
            quint64 max_size = 64;
            inputFile->read(mReadBuffer.data(),max_size);
            emit NotifyReadData(mReadBuffer);
        }
    }
    void CANSocket::onDataRecived(QByteArray data)
    {
        emit NotifyReadData(data);
    }

    void CANSocket::onSendDataRequest(QString dataToSend)
    {
        mSNRead->setEnabled(false);
        mSNWrite->setEnabled(true);
        qDebug() << "socket: " + dataToSend ;
        QByteArray data(dataToSend.toStdString().c_str());
        int res = inputFile->write(data.data(), data.length());
        qDebug() << "result: " << res;
        if(res != -1)
        {
            emit DidWriting(inputFile->handle());
            emit NotifyDataSend(dataToSend);
        }
    }

    bool CANSocket::IsReadyForConnection()
    {
        if( inputFile->exists() && (mSNRead != nullptr) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void CANSocket::Disconnect()
    {
        if(isConnected)
        {
            mSNRead->setEnabled(false);
            mSNWrite->setEnabled(false);
            if(inputFile->isOpen())
            {
                inputFile->close();
                qDebug() << "File successfully closed";
            }
            isConnected=false;
            emit NotifyDisconnected();
        }
        else
        {
            QString msg = "Already disconected";
            emit NotifyStatusChanged(msg);
        }


    }

    void CANSocket::onRefreshDevices()
    {
        checkAvailableUSBPorts();
    }

    bool CANSocket::checkAvailableUSBPorts()
    {
        struct udev *udev;
        struct udev_enumerate *enumerate;
        struct udev_list_entry *devices, *dev_list_entry;
        struct udev_device *dev;
        QStringList devicesList;

        /* Create the udev object */
        udev = udev_new();
        if (!udev)
        {
            qDebug() << ("Can't create udev\n");
            return false;
        }

        /* Create a list of the devices in the 'hidraw' subsystem. */
        enumerate = udev_enumerate_new(udev);
        udev_enumerate_add_match_subsystem(enumerate, "hidraw");
        udev_enumerate_scan_devices(enumerate);
        devices = udev_enumerate_get_list_entry(enumerate);

        udev_list_entry_foreach(dev_list_entry, devices) {
            const char *path;

            /* Get the filename of the /sys entry for the device
               and create a udev_device object (dev) representing it */
            path = udev_list_entry_get_name(dev_list_entry);
            dev = udev_device_new_from_syspath(udev, path);

            /* usb_device_get_devnode() returns the path to the device node
               itself in /dev. */
            devicesList.append(udev_device_get_devnode(dev));
            dev = udev_device_get_parent_with_subsystem_devtype(
                        dev,
                        "usb",
                        "usb_device");
            if (!dev) {
                qDebug() << ("Unable to find parent usb device.");
                exit(1);
            }
            mFoundDevices.setStringList(devicesList);
            udev_device_unref(dev);
        }
        /* Free the enumerator object */
        udev_enumerate_unref(enumerate);
        udev_unref(udev);
        emit NotifyDevicesFound();
        return true;
    }

}
