#include "soundsender.h"

SoundSender::SoundSender(QObject *parent) :
    QBuffer(parent)
{
    udpSocket = new QUdpSocket(this);
}

qint64 SoundSender::writeData(const char *data, qint64 len){

    if (len > 0) {    //send that data!
        qDebug("Sending data : %d",len);
        udpSocket->writeDatagram(data,
                                 len,
                                 QHostAddress::Broadcast,
                                 45454);
    }
    return len;
}


void SoundSender::sendData(QBuffer *buffer) {

    if (buffer->bytesAvailable()) {
        qDebug("we have bytes!");

        QByteArray datagram = buffer->data();
        //QByteArray datagram = "Hello world";
        udpSocket->writeDatagram(datagram.data(),
                                 datagram.size(),
                                 QHostAddress::Broadcast,
                                 45454);

        //Checksum before send
        quint16 val = qChecksum(datagram.constData(),datagram.size());
        qDebug("chk before send : %d",val);

    }
    else {
        qDebug("No bytes");
    }

    delete buffer;
}
