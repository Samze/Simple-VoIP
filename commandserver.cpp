#include "commandserver.h"

CommandServer::CommandServer(QObject *parent) :
    QTcpServer(parent)
{
    //Listen
    //TODO add port
    listen(QHostAddress::Any,12345);

    connect(this,SIGNAL(newConnection()),this,SLOT(receiveConnection()));
}

void CommandServer::receiveConnection() {

    QTcpSocket *receivedSocket = nextPendingConnection();

    qDebug() << "TCPINFO " << receivedSocket->readLine();
   // connect(receivedSocket,SIGNAL(readyRead()), this,SLOT(readRequest()));

}

void CommandServer::readRequest() {


  //  qDebug() << "TCPINFO " << receivedSocket.readLine();

}
