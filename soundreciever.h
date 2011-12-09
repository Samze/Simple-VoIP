#ifndef SOUNDRECIEVER_H
#define SOUNDRECIEVER_H

#include <QUdpSocket>
#include <QBuffer>
//#include "ogg/ogg.h"

class SoundReciever : public QBuffer
{
    Q_OBJECT
public:
    SoundReciever();
protected:
    qint64 readData ( char * data, qint64 len );

public slots:
    void processDatagrams();

private:
    QUdpSocket *udpSocket;



};

#endif // SOUNDRECIEVER_H
