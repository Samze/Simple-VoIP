#include "commandserver.h"

CommandServer::CommandServer(QObject *parent) :
    QTcpServer(parent)
{
    activeConnections = new QList<QTcpSocket *>();
    //TODO add port
    listen(QHostAddress::Any,12345);

    connect(this,SIGNAL(newConnection()),this,SLOT(receiveConnection()));
}

void CommandServer::receiveConnection() {

    receivedSocket = nextPendingConnection();
     activeConnections->append(receivedSocket);

    connect(receivedSocket,SIGNAL(readyRead()), this,SLOT(readRequest()));
}

void CommandServer::readRequest() {

    receivedSocket->peerAddress();
    //qDebug() << "TCPINFO " << receivedSocket->readLine();

    if (receivedSocket->readLine() == "Call") {
        emit callInitiated();
    }


}
