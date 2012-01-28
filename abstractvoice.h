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
	
#ifndef ABSTRACTVOICE_H
#define ABSTRACTVOICE_H

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QDebug>
#include <QRegExp>

/**
 * Abstracted common voice elements for recording and playback.
 * This class contains common attributes and virtual functions for both recording and playing audio.
 * It holds information about the local Audio device detected, a default format and a buffer for which will
 * be written or read.
 */
class AbstractVoice : public QObject
{
    Q_OBJECT

public:
    /**
      * Abstract constructor. Instansiates a format and device object.
      */
    explicit AbstractVoice(QObject *parent = 0);

    /**
      * Abstract destructor. Deletes the device and buffer objects. The buffer
      * object must be instansiated in a subclass.
      */
    ~AbstractVoice();

public slots:
    /**
      * A virtual function that starts the voice operation. This function must
      * be implemented.
      */
    virtual void start() = 0;

    /**
      * A virtual function that stops all voice operation and subsequently is ready to receive another
      * start call. This function must be implemented.
      */
    virtual void stop() = 0;

private slots:
    /**
      * A virtual function that handles a change of state in the audio device. This slot should be
      * implemented and also connected to any device's change of state.
      * @param state Enumerated audio state update.
      */
    virtual void audioState(QAudio::State state) = 0;

protected:
    QAudioFormat format; /**< Audio format to be used, default is set to audio/pcm, 8000 freq, 1 channel, 8 sample rate.*/
    QAudioDeviceInfo *devInfo; /**< Populated with the default audio device detected */
    QBuffer *buffer; /**< The buffer to be used, must be implemented by subclass. */
};

#endif // ABSTRACTVOICE_H
