#include "commandserver.h"

CommandServer::CommandServer(QObject *parent) :
    QTcpServer(parent)
{
    activeConnections = new QList<QTcpSocket *>();
    //TODO add port
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

     QTcpSocket* receivedSocket = nextPendingConnection();
     activeConnections->append(receivedSocket);

     connect(receivedSocket,SIGNAL(readyRead()), this,SLOT(readRequest()));
}

void CommandServer::readRequest() {

    QTcpSocket* receivedSocket = static_cast<QTcpSocket*>(sender());

    QHostAddress receivedAddress = receivedSocket->peerAddress();
    //qDebug() << "TCPINFO " << receivedSocket->readLine();

    QByteArray input = receivedSocket->readLine();

    //Make sure we have reasonable input
    if (input.size() == 1) {
        CommandClient::CallCommands command = static_cast<CommandClient::CallCommands>(*input.data());

        switch(command) {
            case CommandClient::Call:
                emit callInitiated(receivedAddress);
                break;
            case CommandClient::EndCall:
                emit callEnded();
                break;
            case CommandClient::MuteMic:
                emit micMuted();
                break;
            case CommandClient::MuteSound:
                emit soundMuted();
                break;
            default:
                //unknown command, lets tell send that back.
                receivedSocket->write("Unknown Command");
                break;
        }
    }
}

void CommandServer::sendCommand(const QHostAddress &address,ServerResponse response) {

    //Find the tcp connection for the request and reject.
    foreach(QTcpSocket* sock, *activeConnections) {
        if ( sock->peerAddress() == address) {

            char command = static_cast<char>(response);
            sock->write(&command,1);
            break;
        }
    }
}

