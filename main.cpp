#include <QApplication>
#include "startup.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CANMonitor::Startup startUp;
    startUp.show();

    return a.exec();
}
