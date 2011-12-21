#include "receivethread.h"

ReceiveThread::ReceiveThread(QObject *parent) :
    QThread(parent)
{
}

void ReceiveThread::run() {

    VoiceOutput output;

    connect(this,SIGNAL(muteSound(bool)),&output,SLOT(muteSound(bool)));

    output.start();

     //This puts the thread in an event loop, will continue until exit is calld.
    exec();

    output.stop();
    qDebug() << "Finished ouput";
}

void ReceiveThread::listen() {
    start();
}
