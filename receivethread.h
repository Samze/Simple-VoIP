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
	
#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include "voiceoutput.h"

/**
  * A Thread class that controls listening for incoming voice data. This class listens for incoming UDP
  * and then plays it using the Computers default audio out device. This class inherits from QThread.
  */
class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    /**
      * Default constructor.
      *@param parent The owner of this object.
      */
    explicit ReceiveThread(QObject *parent = 0);

protected:
    /**
      *Reimplmented function from QThread that beings receiving and playback.
      */
    void run();

signals:
    void muteSound(bool); /**< Signal that should emitted when a request to mute is processed. This class will then mute the sound. */

public slots:
    /**
      * Starts the new thread.
      */
    void listen();

};

#endif // RECEIVETHREAD_H
