#ifndef VOICEOUTPUT_H
#define VOICEOUTPUT_H

#include "abstractvoice.h"
#include <QIODevice>
#include <QThread>
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>
#include "soundreciever.h"

class VoiceOutput : public AbstractVoice
{
    Q_OBJECT
public:
    explicit VoiceOutput(AbstractVoice *parent = 0);
    ~VoiceOutput();

private:
    QAudioOutput *m_audioOut;

public slots:
    virtual void start();
    virtual void stop();

private slots:
   virtual void audioState(QAudio::State);
   void dataInBuffer();
   void muteSound(bool);

private:
   int timedout;
};

#endif // VOICEOUTPUT_H
