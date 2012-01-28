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
	
#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "commandclient.h"

/**
 * A subclass of QTcpServer that receives command signals from clients over the network.
 * This class will listen on the specificed tcp port (12345) and wait for connections.
 * It will then wait for commands to be send from the client and emit signals
 * for the application to handle. Responses can also be sent back via the server.
 * This class inherits from QTcpServer.
 */
class CommandServer : public QTcpServer
{
    Q_OBJECT
public:

    /**
     * Command Server constructor. Sets up a new command server class.
     * @param parent Parent for this object.
     */
    explicit CommandServer(QObject *parent = 0);

    /**
     * Command Server destructor. Destroys all active connections.
     */
    ~CommandServer();

signals:
    void callInitiated(QHostAddress); /**< Emitted when a call has been initated, contains the address of the caller*/
    void callEnded(); /**< Emitted when the call has been requested to end.*/
    void callerMicMuted(bool); /**< Emitted when the caller has changed the state of their microphone.  */
    void callerSoundMuted(bool); /**< Emitted when the caller has changed the state of their sound playback.*/

private slots:
    /**
      * Handles a new connection. Extracts socket and adds it to list of active connections.
      */
    void receiveConnection();

    /**
      * Handles data sent from the client. Emits signals based on command.
      */
    void readRequest();


public:
    /**
      * Sends command back to the client.
      * @param address The IP of the client to send a message to.
      * @param cmd The command to send. Must be part of the enumerated CallCommand list.
      * @see CommandClient::CallCommand
      */
    void sendCommand(const QHostAddress & address, CommandClient::CallCommand cmd);

private:
    /**
      * A List of all active connections.
      */
    QList<QTcpSocket *> *activeConnections;

};

#endif // COMMANDSERVER_H
