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

/**
  * Captures audio data and puts it in a buffer for sending down the network. This class captures data from the systems native
  * input device and adds it to a buffer to be sent down the network. It is assumed that the format of this audio data is the same as represented
  * by this classes superclass AbstractVoice.
  */
class VoiceInput : public AbstractVoice
{
    Q_OBJECT

public:

    /**
     *Creates our AudioDevice object and buffer used.
     *@param address The address to send captured data.
     */
    explicit VoiceInput(QHostAddress address);

    /**
      *Default destructor
      */
    ~VoiceInput();

public slots:
    /**
      *Implemented virtual start method from AbstractVoice. This method starts recording and filling the network buffer.
      */
   virtual void start();

    /**
      * Implemented virtual stop method from AbstractVoice. This method stops recording.
      */
   virtual void stop();

private slots:

    /**
      * Implemented virtual slot from AbstractVoice. This is connected to changes in our AudioOutput object.
      */
    virtual void audioState(QAudio::State);

    /**
      * Mutes the Mic of sound until toggled via this method.
      */
    void muteMic(bool);

private:
    QAudioInput *m_audioIn; /**< The Input device object. */
};

#endif // VOICEINPUT_H
