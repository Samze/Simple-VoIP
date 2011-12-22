#ifndef VOICEOUTPUT_H
#define VOICEOUTPUT_H

#include "abstractvoice.h"
#include <QIODevice>
#include <QThread>
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>
#include "soundreciever.h"

/**
  * Receives audio data and plays it using the systems default output device. This class receives data from a buffer
  * of audio data and subsequently plays it. It is assumed that the format of this audio data is the same as represented
  * by this classes superclass AbstractVoice.
  *
  */
class VoiceOutput : public AbstractVoice
{
    Q_OBJECT
public:
    /**
     * Default constructor. Creates our AudioDevice object and buffer used.
     *@param parent The owner of this object.
     */
    explicit VoiceOutput(AbstractVoice *parent = 0);

     /**
       *Default destructor
       */
    ~VoiceOutput();

public slots:
    /**
      *Implemented virtual start method from AbstractVoice. This method starts listening trying to get data from the buffer for playback.
      */
    virtual void start();

    /**
      * Implemented virtual stop method from AbstractVoice. This method stops listening.
      */
    virtual void stop();

private slots:

   /**
     * Implemented virtual slot from AbstractVoice. This is connected to changes in our AudioOutput object.
     */
   virtual void audioState(QAudio::State);

    /**
      * Called when data is in the buffer to play. Note there is a check in here that a certain
      * ammount of data is in the buffer before we start playing...This is trying to rid stuttering.
      */
   void dataInBuffer();

   /**
     * Mutes the playback of sound until toggled via this method.
     */
   void muteSound(bool);

private:
   QAudioOutput *audioOut; /**< The Output device object. */
   int timedout; /**< Used for debugging at the moment, but may be required to detect disconnects. */
};

#endif // VOICEOUTPUT_H
