#include "mainview.h"
#include "ui_mainview.h"
#include "View/setuptab.h"

namespace CANMonitor
{
    MainView::MainView(QWidget *parent, SetupTab& setup) :
        QMainWindow(parent),
        mSetupTab(setup),
        ui(new Ui::MainView)
    {
        ui->setupUi(this);
        mSetupTab.setParent(this);
        ui->loSetupTab->addWidget(&mSetupTab);
    }

    MainView::~MainView()
    {
        delete ui;
    }
}
