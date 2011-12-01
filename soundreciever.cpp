#include "soundreciever.h"

SoundReciever::SoundReciever(QObject *parent) :
    QThread(parent)
{

}

void SoundReciever::run(){
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(45454, QUdpSocket::ShareAddress);

        connect(udpSocket, SIGNAL(readyRead()),this,SLOT(processDatagrams()));
}



void SoundReciever::processDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(udpSocket->pendingDatagramSize());
         udpSocket->readDatagram(datagram.data(), datagram.size());
         qDebug("Size : %d",datagram.size());
    }

}
