#ifndef SOUNDSENDER_H
#define SOUNDSENDER_H

#include <QThread>
#include <QBuffer>
#include <QUdpSocket>

class SoundSender : public QThread
{
    Q_OBJECT
public:
    explicit SoundSender(QObject *parent = 0);

protected:
    void run();

private:
    QUdpSocket *udpSocket;

signals:

public slots:
    void sendData(QBuffer*);
};

#endif // SOUNDSENDER_H
