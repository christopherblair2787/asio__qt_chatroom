QT -= gui
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14 console
CONFIG -= app_bundle

QMAKE_CXXFLAGS = -std=c++14

TARGET = Client


DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += include/
INCLUDEPATH += ../include/
INCLUDEPATH += ../boost-1_67/

HEADERS += \
    ../include/message.h \
    client.h \
    include/client.h

SOURCES += \
    src/main.cpp \
    src/client.cpp

LIBS += -L../lib \
        -lboost_system-mgw63-mt-d-x32-1_67 \
        -lboost_thread-mgw63-mt-d-x32-1_67 \
        -lws2_32
