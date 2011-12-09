#ifndef VOICEINPUT_H
#define VOICEINPUT_H

#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QDebug>
#include <QAudioOutput>
#include "soundsender.h"

class VoiceInput : public QObject
{
    Q_OBJECT

public:
    explicit VoiceInput(QObject *parent = 0);
    ~VoiceInput();

public:
    void setDeviceInfo(const QAudioDeviceInfo *info);

public slots:
   void recordSound();
   void stopRecording();

private slots:
   void audioStateSlot(QAudio::State);

signals:
    void hasVoiceData(QBuffer*);

private:
    QAudioFormat m_format;
    QAudioDeviceInfo *m_devInfo;
    QAudioInput *m_audioIn;
    SoundSender *m_soundSender;

};

#endif // VOICEINPUT_H
