#include "soundsender.h"

SoundSender::SoundSender(QObject *parent) :
    QThread(parent)
{
    udpSocket = new QUdpSocket(this);
}


void SoundSender::run(){

}

void SoundSender::sendData(QBuffer *buffer) {

    if (buffer->bytesAvailable()) {
        qDebug("we have bytes!");

        QByteArray datagram = buffer->data();

        udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 QHostAddress::Broadcast,
                                 45454);
    }
    else {
        qDebug("No bytes!");
    }

}
