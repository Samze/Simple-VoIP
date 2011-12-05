#ifndef SOUNDSENDER_H
#define SOUNDSENDER_H

#include <QThread>
#include <QBuffer>
#include <QUdpSocket>
#include <QAudioFormat>

class SoundSender : public QBuffer
{
    Q_OBJECT
public:
    explicit SoundSender(QObject *parent = 0);

protected:
    qint64 writeData (const char * data, qint64 len);

private:
    QUdpSocket *udpSocket;

signals:

public slots:
    void sendData(QBuffer*);
};

#endif // SOUNDSENDER_H
