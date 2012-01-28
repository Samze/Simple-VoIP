/*	
    VoIPApp- A simple p2p voip application written in C++ using the QT Framework.
    Copyright (C) 2011  Sam Gunaratne University of Plymouth

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
	
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
