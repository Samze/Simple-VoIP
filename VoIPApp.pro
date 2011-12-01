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
    voiceoutput.cpp

HEADERS  += mainwindow.h \
    captureaudio.h \
    soundreciever.h \
    soundsender.h \
    voiceoutput.h

FORMS    += mainwindow.ui






