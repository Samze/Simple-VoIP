#ifndef CAPTUREAUDIO_H
#define CAPTUREAUDIO_H

#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QDebug>
#include <QAudioOutput>
#include "soundsender.h"

class CaptureAudio : public QObject
{
    Q_OBJECT

public:
    CaptureAudio();
    ~CaptureAudio();

public:
    void setDeviceInfo(const QAudioDeviceInfo *info);

private slots:
   void recordSound();
   void stopRecording();
   void audioStateSlot(QAudio::State);

signals:
    void hasVoiceData(QBuffer*);

private:
    QAudioFormat m_format;
    QAudioDeviceInfo *m_devInfo;
    QAudioInput *m_audioIn;
    SoundSender *m_soundSender;

};

#endif // CAPTUREAUDIO_H
