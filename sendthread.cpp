#include "sendthread.h"

SendThread::SendThread(QObject *parent) :
    QThread(parent)
{}

SendThread::~SendThread() {
}

void SendThread::run() {

    VoiceInput capture;
    capture.start();

    //Loops thread waiting on events
    int code = exec();

    qDebug() << "Finished capture, tidying up..." << code;
    capture.stop();
}

void SendThread::recordSound() {
    start();
}
