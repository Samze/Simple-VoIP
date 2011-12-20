#include "sendthread.h"

SendThread::SendThread(QObject *parent) :
    QThread(parent)
{}

SendThread::~SendThread() {
}

void SendThread::run() {

    QHostAddress address(m_address);
    VoiceInput input(address);

    connect(this,SIGNAL(muteMic(bool)),&input,SLOT(muteMic(bool)));

    input.start();

    //Loops thread waiting on events
    int code = exec();

    qDebug() << "Finished capture, tidying up..." << code;
    input.stop();
}

void SendThread::recordSound(const QHostAddress &address) {
    m_address = address;
    start();
}
