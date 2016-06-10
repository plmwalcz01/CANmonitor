#pragma once
#include <QObject>
#include <QArrayData>
#include <QThread>
#include <Model/cansocket.h>
#include <Model/canframeformat.h>

class QStringListModel;
namespace CANMonitor{

    class Device : public QObject
    {
        Q_OBJECT
    public:
        explicit Device(QObject *parent);
        virtual ~Device();
        QThread canSocketThread;

        bool Connect(const QString &device);
        bool IsReadyForConnection();
        void Disconnect();

        QStringListModel &getFoundDevices();
    signals:
        void NotifyDataRead(const QString& data);
        void NotifyDataSend(const QString& data);
        void NotifyDataToSend(const QString& data);
        void NotifyErrorDetected(const QString& errMsg);
        void NotifyStatusMsg(const QString& statusMsg);
        void NotifyConnected();
        void NotifyDisonnected();
        void NotifyDevicesFound(QStringListModel &foundDevices);

    public slots:
        void onPWChange();
        void onStatusChanged(QString msg);
        void onDevicesFound();
        void onRefreshDevices();
        void onConnected();
        void onDisonnected();
        void onRecivedData(const QByteArray &readData);
        void onReciveRequest();
        void onBaudRateCahnge();
        void onSamplingPointChange();
        void onSendRequest(const QString &dataToSend);
        void onDataSend(const QString &dataSend);

    private:
        CANSocket mCanSocket;
        void WireConnections();
        QString canFrameToQString(const canBaseFrameFormat &canFrame);
        QString translateCanFrame(QByteArray frame);
        explicit Device(const Device& rhs) = delete;
        Device& operator= (const Device& rhs) = delete;

    };
}



