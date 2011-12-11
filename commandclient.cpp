#include "commandclient.h"

CommandClient::CommandClient(QObject *parent) : //,QString* ip , qint64 port) :
    QObject(parent) //,  m_ip(ip), m_port(port)
{
    m_tcpSocket = new QTcpSocket(this);

 //   connect(m_tcpSocket,SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(m_tcpSocket, SIGNAL(connected()), this,SLOT(sendRequest()));
}

void CommandClient::callUser() {
    m_tcpSocket->connectToHost("127.0.0.1",12345,QIODevice::ReadWrite);


}

void CommandClient::hangUp() {


}


void CommandClient::sendRequest() {
    m_tcpSocket->write("Call");
}
