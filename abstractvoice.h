#ifndef ABSTRACTVOICE_H
#define ABSTRACTVOICE_H

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QDebug>

class AbstractVoice : public QObject
{
    Q_OBJECT

public:
    explicit AbstractVoice(QObject *parent = 0);
    ~AbstractVoice();

public slots:
    virtual void start() = 0;
    virtual void stop() = 0;

private slots:
    virtual void audioState(QAudio::State) = 0;

protected:
    QAudioFormat format;
    QAudioDeviceInfo *devInfo;
    QBuffer *buffer;
};

#endif // ABSTRACTVOICE_H
