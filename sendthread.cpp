#include "sendthread.h"

SendThread::SendThread(QObject *parent) :
    QThread(parent)
{}

void SendThread::run() {

    QHostAddress address(m_address);
    VoiceInput input(address);

    connect(this,SIGNAL(muteMic(bool)),&input,SLOT(muteMic(bool)));

    input.start();

    //Loops thread waiting on events
    int code = exec();

    if(code != 0) {
        qWarning() << "Exited send thread with code " << code;
    }

    input.stop();
}

void SendThread::recordSound(const QHostAddress &address) {
    m_address = address;
    start();
}
