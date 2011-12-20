#include "soundsender.h"

SoundSender::SoundSender(QObject *parent, QHostAddress address) :
    QBuffer(parent), m_address(address)
{
    qDebug() << address;
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

    qDebug() << m_address;

    if (compressed.size() > 0) {    //If we have data - send it!
        qDebug("zSending data : %d",compressed.size());

        udpSocket->writeDatagram(compressed.constData(),
                                 compressed.size(),
                                 m_address,
                                 45454);
    }

    return len;
}
