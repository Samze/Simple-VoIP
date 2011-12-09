#include "soundsender.h"

SoundSender::SoundSender(QObject *parent) :
    QBuffer(parent)
{
    udpSocket = new QUdpSocket(this);
}


qint64 SoundSender::writeData(const char *data, qint64 len){


    //init stream
//    int initRes = ogg_stream_init(&os,123);

//    qDebug("%d", initRes);

//    ogg_packet op;
//    op.packet = (unsigned char*)data;
//    op.bytes = len;
//    if (packetNo == 0)
//        op.b_o_s = 1;
//    else
//        op.b_o_s = 0;
//    op.e_o_s = 0; //TODO send final packet

//    op.granulepos = frameNo;
//    op.packetno = packetNo;

//    //submit packet
//    int packet = ogg_stream_packetin(&os,&op);

//    //check page
//    int pageResult = ogg_stream_pageout(&os,&og);

//    if (pageResult > 0) {
//        qDebug("Data in page %d",og.body_len);
//    }


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
