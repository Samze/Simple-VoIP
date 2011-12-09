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
    soundreciever.cpp \
    soundsender.cpp \
    voiceoutput.cpp \
    receivethread.cpp \
    voiceinput.cpp \
    sendthread.cpp

HEADERS  += mainwindow.h \
    soundreciever.h \
    soundsender.h \
    voiceoutput.h \
    receivethread.h \
    voiceinput.h \
    sendthread.h

FORMS    += mainwindow.ui






