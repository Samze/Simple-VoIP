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
	
#include "commandclient.h"

CommandClient::CommandClient(QObject *parent) :
    QTcpSocket(parent) {

    connect(this,SIGNAL(readyRead()), this, SLOT(serverResponse())); //Slot for receiving command back
    connect(this, SIGNAL(connected()), this,SLOT(callPeer()));
}

void CommandClient::connectToPeer(Peer* peer) {
    connectToHost(*peer->getAddress(),12345,QIODevice::ReadWrite);
}

void CommandClient::sendCommand(CommandClient::CallCommand cmd) {
    char command = static_cast<char>(cmd);
    write(&command,1);
}

void CommandClient::callPeer() {
    sendCommand(CommandClient::Call);
}

void CommandClient::hangUp() {
    sendCommand(CommandClient::EndCall);
    disconnectFromHost();
}

void CommandClient::serverResponse() {

    QByteArray input = readLine();

    //Check input size is correct at least..should add some more advance checks here..
    if (input.size() == 1) {

        CommandClient::CallCommand response = static_cast<CommandClient::CallCommand>(*input.data());
        switch(response) {
            case CommandClient::CallAccepted:
                emit callerAccepted();
                break;
            case CommandClient::Busy:
                emit callerBusy();
                disconnectFromHost();
                break;
            case CommandClient::EndCall:
                emit callEnded();
                disconnectFromHost();
                break;
            case CommandClient::disableMic:
                emit callerMicMuted(true);
                break;
            case CommandClient::enableMic:
                emit callerMicMuted(false);
                break;
            case CommandClient::disableSound:
                emit callerSoundMuted(true);
                break;
            case CommandClient::enableSound:
                emit callerSoundMuted(false);
                break;
            default:
                break;
        }
    }
}

//This method is not currently used.
quint16 CommandClient::generateUDPPort() {
    //num between 0 & 1.
    double rnd = qrand() / static_cast<double> (RAND_MAX);

   quint16 port = UDPMIN + (rnd *(UDPMAX - UDPMIN));

   qDebug() << "Generated Port" << port;
   return port;
}

