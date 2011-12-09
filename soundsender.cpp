#include "soundsender.h"

SoundSender::SoundSender(QObject *parent) :
    QBuffer(parent)
{
    udpSocket = new QUdpSocket(this);
}

SoundSender::~SoundSender() {
    delete udpSocket;
}

qint64 SoundSender::writeData(const char *data, qint64 len){

    QByteArray uncompressed(data,len);

    QByteArray compressed = qCompress(uncompressed,9);

    float compPercent = ((float)compressed.size()/len) * 100;
    qDebug() << "Compressed " << len << " bytes into " << compressed.size() << " giving " << 100 - compPercent << "% compression rate";


    if (compressed.size() > 0) {    //If we have data - send it!
        qDebug("zSending data : %d",compressed.size());
        udpSocket->writeDatagram(compressed.constData(),
                                 compressed.size(),
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
