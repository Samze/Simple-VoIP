#include "sendthread.h"

SendThread::SendThread(QObject *parent) :
    QThread(parent)
{}

SendThread::~SendThread() {
}

void SendThread::run() {

    VoiceInput capture(m_address);
    capture.start();

    qDebug() << m_address;
    //Loops thread waiting on events
    int code = exec();

    qDebug() << "Finished capture, tidying up..." << code;
    capture.stop();
}

void SendThread::recordSound(QHostAddress address) {
    m_address = address;
    start();
}
