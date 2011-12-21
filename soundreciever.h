#ifndef SOUNDRECIEVER_H
#define SOUNDRECIEVER_H

#include <QUdpSocket>
#include <QBuffer>
//#include "ogg/ogg.h"

/**
  * A buffer that receives UDP audio data over the network. This class listens on a specific UDP port for audio data
  * sent by another client. It decompresses it and adds it to it's buffer. Read requests on this buffer will empty it.
  * This class inherits from QBuffer.
  */
class SoundReciever : public QBuffer
{
    Q_OBJECT
public:

    /**
      *Default constructor which binds to a UDP port.
      *@param parent The owner of this object.
      */
    explicit SoundReciever(QObject *parent = 0);

    /**
      * Deletes the UDPSocket.
      */
    ~SoundReciever();

protected:
    /**
      * Overridden QBuffer method. This method will empty the buffer on a read request.
      * @param data A pointer to data where it should be placed.
      * @param len The length of data to try and read
      * @return The length actually read into data.
      * @see QBuffer::readData()
      */
    qint64 readData ( char * data, qint64 len );

private slots:
    /**
      * Called everytime new data is received. This method will decompress the data and add it to our buffer.
      * @see QBuffer::buffer()
      */
    void processDatagrams();

private:
    QUdpSocket *udpSocket; /**< The UDP socket used. */

};

#endif // SOUNDRECIEVER_H
