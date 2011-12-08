#include "soundsender.h"

SoundSender::SoundSender(QObject *parent) :
    QBuffer(parent)
{
    udpSocket = new QUdpSocket(this);
}


qint64 SoundSender::writeData(const char *data, qint64 len){

    if (len > 0) {    //If we have data - send it!
        qDebug("Sending data : %d",len);
        udpSocket->writeDatagram(data,
                                 len,
                                 QHostAddress::Broadcast,
                                 45454);
    }

    return len;
}

//The follow code creates a mono-tone, should be used for testing.

//QByteArray ba;
//ba.resize(len);

//for (int i = 0;i < len; i = i +2) {
//    ba[i] = 127;
//    ba[i+1] = -127;
//}
