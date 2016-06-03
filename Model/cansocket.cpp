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

        if(!inputFile->open(QFile::ReadOnly))
        {
            std::cout << "Cant open!" << qPrintable(device);
            std::cout << qPrintable(inputFile->errorString()) ;
            return false;
        }

        mSNRead = new QSocketNotifier(inputFile->handle(), QSocketNotifier::Read);
        mSNRead->setEnabled(true);
        QObject::connect(mSNRead, SIGNAL(activated(int)), this, SLOT(readyRead()));
        emit NotifyConnected();
        return true;
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
        mSNRead->setEnabled(false);
        if(inputFile->isOpen())
        {
            inputFile->close();
            qDebug() << "File successfully closed";
        }
        emit NotifyDisconnected();
    }
    void CANSocket::readyRead()
    {
        mReadBuffer = inputFile->read(8);
        mReadData = mReadBuffer.toHex();
        emit NotifyReadData(mReadData);
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
        if (!udev) {
            qDebug() << ("Can't create udev\n");
            return false;
        }

        /* Create a list of the devices in the 'hidraw' subsystem. */
        enumerate = udev_enumerate_new(udev);
        udev_enumerate_add_match_subsystem(enumerate, "hidraw");
        udev_enumerate_scan_devices(enumerate);
        devices = udev_enumerate_get_list_entry(enumerate);

        /* For each item enumerated, print out its information.
           udev_list_entry_foreach is a macro which expands to
           a loop. The loop will be executed for each member in
           devices, setting dev_list_entry to a list entry
           which contains the device's path in /sys. */
        udev_list_entry_foreach(dev_list_entry, devices) {
            const char *path;

            /* Get the filename of the /sys entry for the device
               and create a udev_device object (dev) representing it */
            path = udev_list_entry_get_name(dev_list_entry);
            dev = udev_device_new_from_syspath(udev, path);

            /* usb_device_get_devnode() returns the path to the device node
               itself in /dev. */
            printf("Device Node Path: %s\n", udev_device_get_devnode(dev));
            devicesList.append(udev_device_get_devnode(dev));


            /* The device pointed to by dev contains information about
               the hidraw device. In order to get information about the
               USB device, get the parent device with the
               subsystem/devtype pair of "usb"/"usb_device". This will
               be several levels up the tree, but the function will find
               it.*/
            dev = udev_device_get_parent_with_subsystem_devtype(
                   dev,
                   "usb",
                   "usb_device");
            if (!dev) {
                printf("Unable to find parent usb device.");
                exit(1);
            }

            /* From here, we can call get_sysattr_value() for each file
               in the device's /sys entry. The strings passed into these
               functions (idProduct, idVendor, serial, etc.) correspond
               directly to the files in the directory which represents
               the USB device. Note that USB strings are Unicode, UCS2
               encoded, but the strings returned from
               udev_device_get_sysattr_value() are UTF-8 encoded. */
            printf("  %s\n  %s\n",
                    udev_device_get_sysattr_value(dev,"manufacturer"),
                    udev_device_get_sysattr_value(dev,"product"));
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
