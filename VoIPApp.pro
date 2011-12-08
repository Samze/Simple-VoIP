#-------------------------------------------------
#
# Project created by QtCreator 2011-11-24T10:13:44
#
#-------------------------------------------------

QT       += core gui multimedia network

TARGET = VoIPApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    captureaudio.cpp \
    soundreciever.cpp \
    soundsender.cpp \
    voiceoutput.cpp \
    receivethread.cpp

HEADERS  += mainwindow.h \
    captureaudio.h \
    soundreciever.h \
    soundsender.h \
    voiceoutput.h \
    receivethread.h

FORMS    += mainwindow.ui

win32: LIBS += -L$$PWD/ -llibogg_static

INCLUDEPATH += $$PWD/C:/Users/Sam/Downloads/libogg-1.3.0/libogg-1.3.0/include
DEPENDPATH += $$PWD/C:/Users/Sam/Downloads/libogg-1.3.0/libogg-1.3.0/include

win32: PRE_TARGETDEPS += $$PWD/libogg_static.lib

win32: LIBS += -L$$PWD/libspeex/ -llibspeex

INCLUDEPATH += $$PWD/libspeex/include
DEPENDPATH += $$PWD/libspeex/include

win32: PRE_TARGETDEPS += $$PWD/libspeex/libspeex.lib
