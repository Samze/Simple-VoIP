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
	
#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include "voiceinput.h"


/**
  * A Thread class that controls recording audio and sending data communication. This class records audio from the
  * default recording device and the sends them via UDP to the destination specified. This class inherits from QThread.
  */
class SendThread : public QThread
{
    Q_OBJECT
public:

    /**
      * Default constructor.
      *@param parent The owner of this object.
      */
    explicit SendThread(QObject *parent = 0);

signals:
    void muteMic(bool); /**< Signal that should emitted when a request to mute the mic is processed. This class will then mute the microphone. */

    /**
      *Reimplmented function from QThread that beings receiving and playback.
      */
protected:
    void run();

    /**
      * Starts the new thread, sending data to the given address.
      *@param addr The address to send the recorded audio.
      */
public slots:
    void recordSound(const QHostAddress & addr);

private:
    QHostAddress address; /**< locally stored address for thread access. */
};

#endif // SENDTHREAD_H
