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
    explicit VoiceOutput(QObject *parent = 0);
    ~VoiceOutput();

private:
    QAudioOutput *m_audioOut;
    QAudioFormat m_format;
    QAudioDeviceInfo *m_devInfo;
    SoundReciever* m_soundReceiver;

public slots:
    void playSound();
    void finishedPlaying(QAudio::State);
    void dataInBuffer();
};

#endif // VOICEOUTPUT_H
