#ifndef SOUNDSENDER_H
#define SOUNDSENDER_H

#include <QThread>
#include <QBuffer>
#include <QUdpSocket>
#include <QAudioFormat>
//#include "ogg/ogg.h"

class SoundSender : public QBuffer
{
    Q_OBJECT
public:
    explicit SoundSender(QObject *parent, QHostAddress& address);
    ~SoundSender();

protected:
    qint64 writeData (const char * data, qint64 len);

private:
    QUdpSocket *udpSocket;
    QHostAddress m_address;
    //TODO make qint64
    int packetNo;
    int frameNo;
    int pageNo;
//    ogg_page  og;
//    ogg_stream_state os;

};

#endif // SOUNDSENDER_H
