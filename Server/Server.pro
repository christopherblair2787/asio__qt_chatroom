QT -= gui
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14 console
CONFIG -= app_bundle

QMAKE_CXXFLAGS = -std=c++14

TARGET = Server


DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += include/
INCLUDEPATH += ../include/
INCLUDEPATH += ../boost-1_67/

HEADERS += \
    ../include/message.h \
    include/server.h \
    include/participant.h \
    include/room.h \
    include/session.h

SOURCES += \
    src/main.cpp \
    src/room.cpp \
    src/session.cpp \
    src/server.cpp


LIBS += -L../lib \
        -lboost_system-mgw63-mt-d-x32-1_67 \
        -lws2_32 \
        -lwsock32
