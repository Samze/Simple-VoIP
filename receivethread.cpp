#include "receivethread.h"

ReceiveThread::ReceiveThread(QObject *parent) :
    QThread(parent)
{
}


void ReceiveThread::run() {

     VoiceOutput* output = new VoiceOutput();
     output->playSound();

     //This puts the thread in an event loop, will continue until exit is calld.
     exec();
}

void ReceiveThread::listen() {
    start();
}
