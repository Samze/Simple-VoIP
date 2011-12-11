#include "commandclient.h"

CommandClient::CommandClient(QObject *parent,QString* ip, qint64 port) :
    QObject(parent),  m_ip(ip), m_port(port)
{
    m_tcpSocket = new QTcpSocket(this);

    connect(m_tcpSocket,SIGNAL(readyRead()), this, SLOT(readFortune()));
}

void CommandClient::callUser() {


}

void CommandClient::hangUp() {


}
