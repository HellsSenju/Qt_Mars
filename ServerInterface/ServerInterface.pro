QT       += core gui websockets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerInterface
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    group.cpp \
    iperf.cpp \
    customlist.cpp

HEADERS += \
        mainwindow.h \
    group.h \
    iperf.h \
    customlist.h


FORMS += \
        mainwindow.ui


unix:!macx: LIBS += -L$$PWD/../../Libs/build-SettingsLib-Desktop-Debug/ -lSettingsLib

INCLUDEPATH += $$PWD/../../Libs/SettingsLib
DEPENDPATH += $$PWD/../../Libs/SettingsLib

unix:!macx: LIBS += -L$$PWD/../../Libs/build-DBLib-Desktop-Debug/ -lDBLib

INCLUDEPATH += $$PWD/../../Libs/DBLib
DEPENDPATH += $$PWD/../../Libs/DBLib

unix:!macx: LIBS += -L$$PWD/../../libwebsocketcore-build/server-build/ -lWSSC_as_Library

INCLUDEPATH += $$PWD/../../libwebsocketcore-build/server
DEPENDPATH += $$PWD/../../libwebsocketcore-build/server
