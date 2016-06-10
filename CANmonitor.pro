#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T12:20:03
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CANmonitor
TEMPLATE = app

QMAKE_CXXFLAGS = -std=c++11

SOURCES += main.cpp\
        View/mainview.cpp \
    View/setuptab.cpp \
    startup.cpp \
    utils.cpp \
    Model/settings.cpp \
    Model/provider.cpp \
    Model/cansocket.cpp \
    Model/device.cpp \
    ViewMgr/setubtabviewmanager.cpp \
    View/controltab.cpp \
    ViewMgr/controltabviewmanager.cpp

HEADERS  += View/mainview.h \
    View/setuptab.h \
    startup.h \
    utils.h \
    Model/settings.h \
    Model/provider.h \
    Model/cansocket.h \
    Model/device.h \
    ViewMgr/setubtabviewmanager.h \
    ViewMgr/controltabviewmanager.h \
    View/controltab.h \
    Model/canframeformat.h

FORMS    += View/mainview.ui \
    View/setuptab.ui \
    View/controltab.ui

RESOURCES += \
    CANmonitor_resources.qrc
