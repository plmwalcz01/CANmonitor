#include "mainview.h"
#include "ui_mainview.h"
#include "View/setuptab.h"
#include "View/controltab.h"

namespace CANMonitor
{
    MainView::MainView(QWidget *parent, SetupTab& setup, CANMonitor::ControlTab &control) :
        QMainWindow(parent),
        mSetupTab(setup),
        mControlTab(control),
        ui(new Ui::MainView)
    {
        ui->setupUi(this);
        mSetupTab.setParent(this);
        mControlTab.setParent(this);
        ui->loSetupTab->addWidget(&mSetupTab);
        ui->loControlTab->addWidget(&mControlTab);
    }

    MainView::~MainView()
    {
        delete ui;
    }
}
