#pragma once
#include <QObject>
#include <QArrayData>

class QStringListModel;
namespace CANMonitor{


    class CANSocket;
    class Device : public QObject
    {
        Q_OBJECT
    public:
        explicit Device(QObject *parent, CANSocket& CANSock);
        virtual ~Device();

        bool Connect(const QString &device);
        bool IsReadyForConnection();
        void Disconnect();
        QStringListModel &getFoundDevices();
    signals:
        void NotifyDataRead(const QString& data);
        void NotifyDataSend(const QString& data);
        void NotifyErrorDetected(const QString& errMsg);
        void NotifyStatusMsg(const QString& statusMsg);
        void NotifyConnected();
        void NotifyDisonnected();
        void NotifyDevicesFound(QStringListModel &foundDevices);

    public slots:
        void onDevicesFound();
        void onRefreshDevices();
        void onConnected();
        void onDisonnected();
        void onRecivedData(const QString &readData);
        void onReciveRequest();
        void onBaudRateCahnge();
        void onSamplingPointChange();
        void onSendRequest(const QString &dataToSend);

    private:
        CANSocket& mCanSocket;

        void WireConnections();

        explicit Device(const Device& rhs) = delete;
        Device& operator= (const Device& rhs) = delete;
    };
}



