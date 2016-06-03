#include "setuptab.h"
#include "ui_setuptab.h"
#include "utils.h"
#include <iostream>
#include <QDebug>
#include <QStringListModel>
#include <QPair>

namespace CANMonitor
{
    SetupTab::SetupTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SetupTab)
    {
        ui->setupUi(this);
    }

    SetupTab::~SetupTab()
    {
        Utils::DestructorMsg(this);
        delete ui;
    }

    void SetupTab::setBaudRate(const QString &baudRate)
    {
        ui->editBaudRate->setText(baudRate);
        emit NotifyBaudRateChanged(baudRate);
    }

    void SetupTab::setSamplingPoint(const int &samplingPoint)
    {
        ui->editSamplingPoint->setText(QString::number(samplingPoint));
        emit NotifySamplingPointChanged(samplingPoint);
    }

    void SetupTab::setFoundDevices(QStringListModel &deviceList)
    {
        ui->cmbDevices->setModel(&deviceList);
    }
    void SetupTab::setCommands(QStringListModel &commands)
    {
        ui->cmbMessages->setModel(&commands);
    }

    void SetupTab::onStatusUpdated(const QString &statusMsg)
    {
        ui->editCANMessages->append(statusMsg);
    }

    void SetupTab::onFoundDevices(QStringListModel &devices)
    {
        setFoundDevices(devices);
    }

    void SetupTab::onConnected()
    {
        QString msg("Connected");
        ui->editCANMessages->append(msg);
    }

    void SetupTab::onDisonnected()
    {
        QString msg("Disconnected");
        ui->editCANMessages->append(msg);
    }

    void SetupTab::onDataSend(const QString &dataSend)
    {
        QString tmpStr = ("send: " + dataSend);
        ui->editCANMessages->append(tmpStr);
    }

    void SetupTab::onDataRecived(const QString &dataRecived)
    {
        recCount++;
        mtmpString.append(dataRecived + "\n");
        if(recCount == 20)
        {
            ui->editCANMessages->append(mtmpString);
            mtmpString.clear();
            recCount=0;
        }
    }
    void SetupTab::on_editBaudRate_editingFinished()
    {
        emit NotifyBaudRateChanged(ui->editBaudRate->text());
    }
    void SetupTab::on_editSamplingPoint_editingFinished()
    {
        bool ok;
        QString enteredText = ui->editSamplingPoint->text();
        int result = enteredText.toInt(&ok);

        if(!ok || (result > 100)) // cant be more than 100%
        {
            ui->editCANMessages->append("Invalid sampling point " + enteredText);
        }
        else
        {
            emit NotifySamplingPointChanged(result);
        }
    }
    void SetupTab::on_btnRefresh_clicked()
    {
        emit NotifyRefreshDevices();
    }
    void SetupTab::on_btnConnect_clicked()
    {
        emit NotifyConnectedClicked(ui->cmbDevices->currentText());
    }
    void SetupTab::on_btnDisconnect_clicked()
    {
        emit NotifyDisconnectedClicked();
    }
    void SetupTab::on_btnSend_clicked()
    {
        emit NotifySendClicked(ui->cmbMessages->currentText());
    }
    void SetupTab::on_btnReceive_clicked()
    {
        emit NotifyReciveClicked();
    }
    void SetupTab::on_btnClear_clicked()
    {
        ui->editCANMessages->clear();
    }
}

