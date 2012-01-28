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
	
#include "commandserver.h"

CommandServer::CommandServer(QObject *parent) :
    QTcpServer(parent)
{
    activeConnections = new QList<QTcpSocket *>();
    listen(QHostAddress::Any,12345);

    connect(this,SIGNAL(newConnection()),this,SLOT(receiveConnection()));
}

CommandServer::~CommandServer(){
    //Not sure what the best practice is for this...
    foreach(QTcpSocket* tcpSock, *activeConnections) {
        tcpSock->disconnectFromHost();
        tcpSock->deleteLater();
    }
    delete activeConnections;
}

void CommandServer::receiveConnection() {

     //Get our connect and add them to a list.
     QTcpSocket* receivedSocket = nextPendingConnection();

     activeConnections->append(receivedSocket);

     connect(receivedSocket,SIGNAL(readyRead()), this,SLOT(readRequest()));
}

void CommandServer::sendCommand(const QHostAddress &address,CommandClient::CallCommand cmd) {

    //Find the tcp connection for the request and reject.
    foreach(QTcpSocket* sock, *activeConnections) {
        if ( sock->peerAddress() == address) {

            char command = static_cast<char>(cmd);
            sock->write(&command,1);
            break;
        }
    }
}

void CommandServer::readRequest() {

    QTcpSocket* receivedSocket = static_cast<QTcpSocket*>(sender());

    QHostAddress receivedAddress = receivedSocket->peerAddress();
    //qDebug() << "TCPINFO " << receivedSocket->readLine();

    QByteArray input = receivedSocket->readLine();

    //Make sure we have reasonable input
    if (input.size() == 1) {
        CommandClient::CallCommand command = static_cast<CommandClient::CallCommand>(*input.data());

        switch(command) {
            case CommandClient::Call:
            emit callInitiated(receivedAddress);
            break;
            case CommandClient::EndCall:
                emit callEnded();
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
                //TODO send error code
                break;
        }
    }
}

