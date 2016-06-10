#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>


namespace Ui {
    class MainView;
}

namespace CANMonitor
{
    class SetupTab;
    class ControlTab;

    class MainView : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainView(QWidget *parent, CANMonitor::SetupTab &setup, CANMonitor::ControlTab &control);
        ~MainView();

    private:
        SetupTab& mSetupTab;
        ControlTab& mControlTab;
        Ui::MainView *ui;
    };
}
#endif // MAINVIEW_H
