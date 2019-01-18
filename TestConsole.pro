#-------------------------------------------------
#
# Project created by QtCreator 2018-08-27T14:15:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestConsole
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    led.cpp \
    mav.cpp \
    mav_udp.cpp \
    uart_robot.cpp \
    xml.cpp \
    TestResult.cpp

HEADERS  += mainwindow.h \
    led.h \
    mav.h \
    mav_udp.h \
    uart_robot.h \
    xml.h \
    TestResult.h

FORMS    += mainwindow.ui \
    dialog_test_result.ui

QT += network widgets

QT += xml

QT += webkit

QT += serialport


INCLUDEPATH += mavlink

RESOURCES += \
    image.qrc
