#pragma once
#include <QObject>
#include <QList>
#include <linux/hiddev.h>
#include <libudev.h>
#include <QSocketNotifier>
#include <QFile>
#include <QPointer>
#include <QDebug>
#include <QVector>
#include <QPair>
#include <QStringListModel>


namespace CANMonitor{

    class CANSocket : public QObject
    {
        Q_OBJECT
    public:

        explicit CANSocket(QObject *parent=0);
        QFile *inputFile;
        QPointer<QSocketNotifier> mSNRead = nullptr;
        QPointer<QSocketNotifier> mSNWrite = nullptr;

        bool isConnected = false;

        QString mLastPluggedDevice;
        bool Connect();
        bool IsReadyForConnection();
        void Disconnect();

        void WriteData(const QString &data) const;
        bool Connect(QString device);
        ~CANSocket();
        QByteArray getReadData();
        QString getReadDataAsQString();
        QStringListModel &getFoundDevices();
        bool checkAvailableUSBPorts();

    public slots:
        void readyRead();
        void onRefreshDevices();
        void onDataRecived(QByteArray data);
        void onSendDataRequest(QString dataToSend);
        void readySend();
    signals:
        void NotifyDevicesFound();
        void NotifyReadData(QByteArray readData);
        void NotifyStatusChanged(QString &statusMsg);
        void NotifyDataSend(const QString &dataSend);
        void NotifyConnected();
        void NotifyDisconnected();
        void DidWriting(int ok);


    private slots:
        //void onRecivedData();
        //void onDisconnected();

    private:

        QString mReadData;
        QByteArray mReadBuffer;
        QStringListModel &mFoundDevices;
        explicit CANSocket(const CANSocket& rhs) = delete;
        CANSocket& operator= (const CANSocket& rhs) = delete;
    };
}



