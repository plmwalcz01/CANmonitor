#ifndef SETUPTAB_H
#define SETUPTAB_H

#include <QWidget>
#include <QByteArray>
#include <QVector>

class QStringListModel;

namespace Ui {
    class SetupTab;
}
namespace CANMonitor
{
    class SetupTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit SetupTab(QWidget *parent = 0);
        ~SetupTab();

        void setBaudRate(const QString& baudRate);
        void setSamplingPoint(const int& samplingPoint);
        void setFoundDevices(QStringListModel &devices);
        void setCommands(QStringListModel& commands);
    public slots:
        void onStatusUpdated(const QString &statusMsg);
        void onFoundDevices(QStringListModel &devices);
        void onConnected();
        void onDisonnected();
        void onDataSend(const QString& dataSend);
        void onDataRecived(const QString& dataRecived);
    signals:
        void NotifyBaudRateChanged(const QString& baudRate);
        void NotifySamplingPointChanged(int samplingPoint);
        void NotifyRefreshDevices();
        void NotifyConnectedClicked(const QString &selectedDevice);
        void NotifyDisconnectedClicked();
        void NotifySendClicked(const QString& messageToSend);
        void NotifyReciveClicked();


    private slots:
        void on_editBaudRate_editingFinished();
        void on_editSamplingPoint_editingFinished();
        void on_btnRefresh_clicked();
        void on_btnConnect_clicked();
        void on_btnDisconnect_clicked();
        void on_btnClear_clicked();
        void on_btnSend_clicked();
        void on_btnReceive_clicked();

    private:
        int recCount = 0;
        QString mtmpString;
        Ui::SetupTab *ui;

    };

}
#endif // SETUPTAB_H
