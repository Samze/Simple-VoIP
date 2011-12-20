#ifndef VOICEINPUT_H
#define VOICEINPUT_H

#include "abstractvoice.h"
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QDebug>
#include <QAudioOutput>
#include "soundsender.h"

class VoiceInput : public AbstractVoice
{
    Q_OBJECT

public:
//    explicit VoiceInput(AbstractVoice *parent = 0);
    explicit VoiceInput(QHostAddress address);
    ~VoiceInput();

public:
    void setDeviceInfo(const QAudioDeviceInfo *info);

public slots:
   virtual void start();
   virtual void stop();

private slots:
    virtual void audioState(QAudio::State);

private:
    QAudioInput *m_audioIn;

};

#endif // VOICEINPUT_H
