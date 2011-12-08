#include "soundsender.h"

SoundSender::SoundSender(QObject *parent) :
    QBuffer(parent)
{
    udpSocket = new QUdpSocket(this);
}


qint64 SoundSender::writeData(const char *data, qint64 len){


//    SpeexBits bits;
//    void *enc_state;
//    int quality = 5;
//    char * encodedAud;
//    int MAX_NB_BYTES = 320;
//    int frame_size;

//    speex_bits_init(&bits);
//    enc_state = speex_encoder_init(&speex_nb_mode);

//    speex_encoder_ctl(enc_state,SPEEX_GET_FRAME_SIZE,&frame_size);

//    speex_encoder_ctl(enc_state,SPEEX_SET_QUALITY,&quality);


//    speex_bits_reset(&bits);
//    speex_encode(enc_state, (float*)data, &bits);
//    int nbBytes = speex_bits_write(&bits, encodedAud, MAX_NB_BYTES);

    //OGG time
    frameNo++;
    packetNo++;

    //init stream
    int initRes = ogg_stream_init(&os,123);

    qDebug("%d", initRes);

    ogg_packet op;
    op.packet = (unsigned char*)data;
    op.bytes = len;
    if (packetNo == 0)
        op.b_o_s = 1;
    else
        op.b_o_s = 0;
    op.e_o_s = 0; //TODO send final packet

    op.granulepos = frameNo;
    op.packetno = packetNo;

    //submit packet
    int packet = ogg_stream_packetin(&os,&op);

    //check page
    int pageResult = ogg_stream_pageout(&os,&og);

    if (pageResult > 0) {
        qDebug("Data in page %d",og.body_len);
    }


    if (len > 0) {    //If we have data - send it!
        qDebug("zSending data : %d",len);
        udpSocket->writeDatagram(data,
                                 len,
                                 QHostAddress::Broadcast,
                                 45455);
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
