#ifndef SOUNDRECIEVER_H
#define SOUNDRECIEVER_H

#include <QThread>
#include <QUdpSocket>

class SoundReciever : public QThread
{
    Q_OBJECT
public:
    explicit SoundReciever(QObject *parent = 0);

protected:
    void run();

signals:

public slots:
    void processDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // SOUNDRECIEVER_H
