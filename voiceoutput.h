#ifndef VOICEOUTPUT_H
#define VOICEOUTPUT_H

#include <QIODevice>
#include <QThread>
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>
#include "soundreciever.h"

class VoiceOutput : public QObject
{
    Q_OBJECT
public:
    VoiceOutput();
    QBuffer *buffer; //temp

private:
    QAudioOutput *audOut;
    QAudioFormat format;
    QAudioDeviceInfo *devInfo;
    SoundReciever* soundRec;
signals:

public slots:
    void playSound();
    void finishedPlaying(QAudio::State);
    void dataInBuffer();
};

#endif // VOICEOUTPUT_H
