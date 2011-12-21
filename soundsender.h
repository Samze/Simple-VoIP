#ifndef SOUNDSENDER_H
#define SOUNDSENDER_H

#include <QThread>
#include <QBuffer>
#include <QUdpSocket>
#include <QAudioFormat>
//#include "ogg/ogg.h"

/**
  * A buffer that sends UDP audio data over the network. This class sends  audio data down a specific UDP port
  * to another client. When data is written to this buffer, it is immenditely compressed and sent down the network.
  */
class SoundSender : public QBuffer
{
    Q_OBJECT

public:

    /**
      *Default constructor which binds to a UDP port.
      *@param parent The owner of this object.
      *@param address The address to send the data.
      */
    explicit SoundSender(QObject *parent, QHostAddress address);

    /**
      * Deletes the UDPSocket.
      */
    ~SoundSender();

protected:
    /**
      * Overridden from QBuffer, when data is written to this buffer it is sent down the network to the
      * address specified.
      * @param data The data to put into the buffer.
      * @param len The length of data to add.
      * @return The length of data sent.
      */
    qint64 writeData (const char * data, qint64 len);

private:
    QUdpSocket *udpSocket; /**< The udp socket used. */
    QHostAddress m_address; /**< The address to send the data. */

//Was playing around using OGG transport layer and speex compression...lots of effort..
//    int packetNo;
//    int frameNo;
//    int pageNo;
//    ogg_page  og;
//    ogg_stream_state os;

};

#endif // SOUNDSENDER_H
