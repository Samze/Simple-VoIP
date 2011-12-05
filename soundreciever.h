#ifndef SOUNDRECIEVER_H
#define SOUNDRECIEVER_H

#include <QUdpSocket>
#include <QBuffer>

class SoundReciever : public QBuffer
{
    Q_OBJECT
public:
    SoundReciever();
    QUdpSocket *udpSocket;
signals:
    void dataReceived(QBuffer*);

protected:
    qint64 readData ( char * data, qint64 len );

public slots:
    void processDatagrams();

};

#endif // SOUNDRECIEVER_H
