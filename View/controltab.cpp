#include "controltab.h"
#include "ui_controltab.h"
#include "utils.h"

namespace CANMonitor {

    ControlTab::ControlTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ControlTab)
    {
        ui->setupUi(this);
    }

    ControlTab::~ControlTab()
    {
        Utils::DestructorMsg(this);
        delete ui;
    }

    void ControlTab::onControlTabEnabled(bool isEnabled)
    {
        ui->ControlTabPanel->setEnabled(isEnabled);
    }
    void ControlTab::on_spinPW_valueChanged(double value)
    {
        emit NotifyPWChange(value);
    }
}


