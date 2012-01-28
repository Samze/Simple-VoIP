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
	
#include "receivethread.h"

ReceiveThread::ReceiveThread(QObject *parent) :
    QThread(parent)
{
}

void ReceiveThread::run() {

    VoiceOutput output;

    connect(this,SIGNAL(muteSound(bool)),&output,SLOT(muteSound(bool)));

    output.start();

     //This puts the thread in an event loop, will continue until exit is calld.
    int code = exec();

    if(code != 0) {
        qWarning() << "Exited receive thread with code " << code;
    }

    output.stop();
    qDebug() << "Finished ouput";
}

void ReceiveThread::listen() {
    start();
}
