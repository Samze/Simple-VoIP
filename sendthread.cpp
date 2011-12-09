#include "sendthread.h"

SendThread::SendThread(QObject *parent) :
    QThread(parent)
{}

SendThread::~SendThread() {
}

void SendThread::run() {

    VoiceInput capture;
    capture.recordSound();

    //Loops thread waiting on events
    int code = exec();

    qDebug() << "Finished capture, tidying up..." << code;
    capture.stopRecording();
}

void SendThread::recordSound() {
    start();
}
