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
	
#include "sendthread.h"

SendThread::SendThread(QObject *parent) :
    QThread(parent)
{}

void SendThread::run() {

    QHostAddress addr(address);
    VoiceInput input(addr);

    connect(this,SIGNAL(muteMic(bool)),&input,SLOT(muteMic(bool)));

    input.start();

    //Loops thread waiting on events
    int code = exec();

    if(code != 0) {
        qWarning() << "Exited send thread with code " << code;
    }

    input.stop();
}

void SendThread::recordSound(const QHostAddress &addr) {
    address = addr;
    start();
}
