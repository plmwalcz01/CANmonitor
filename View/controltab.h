#ifndef CONTROLTAB_H
#define CONTROLTAB_H

#include <QWidget>

namespace Ui {
    class ControlTab;
}

namespace CANMonitor {


    class ControlTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit ControlTab(QWidget *parent = 0);
        ~ControlTab();

    public slots:
        void onControlTabEnabled(bool isEnabled);
    private slots:
        void on_spinPW_valueChanged(double value);

    signals:
        void NotifyPWChange(double value);
    private:
        Ui::ControlTab *ui;
    };
}
#endif // CONTROLTAB_H
