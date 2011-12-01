#ifndef CAPTUREAUDIO_H
#define CAPTUREAUDIO_H

#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QDebug>
#include <QAudioOutput>

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
   void playSound(QBuffer*);
   void finishedPlaying(QAudio::State);

signals:
    void hasVoiceData(QBuffer*);

private:
    QAudioFormat format;
    QAudioDeviceInfo *devInfo;
    QAudioInput *audioDev;
    QBuffer *buffer;
    QAudioOutput *audOut;

};

#endif // CAPTUREAUDIO_H
