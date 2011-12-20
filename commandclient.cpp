#include "commandclient.h"

CommandClient::CommandClient(QObject *parent) :
    QTcpSocket(parent) {

    connect(this,SIGNAL(readyRead()), this, SLOT(serverResponse()));
    connect(this, SIGNAL(connected()), this,SLOT(callPeer()));
}

void CommandClient::connectToPeer() {
    connectToHost("141.163.48.92",12345,QIODevice::ReadWrite);
}

void CommandClient::connectToPeer(Peer* peer) {

    connectToHost(*peer->getAddress(),12345,QIODevice::ReadWrite);
}

void CommandClient::hangUp() {
    char command = static_cast<char>(CommandClient::EndCall);
    write(&command,1);
}

void CommandClient::callPeer() {
    char command = static_cast<char>(CommandClient::Call);

    write(&command,1);
}

void CommandClient::serverResponse() {

    QByteArray input = readLine();

    //check input
    if (input.size() == 1) {
        CommandServer::ServerResponse response = static_cast<CommandServer::ServerResponse>(*input.data());

        switch(response) {
            case CommandServer::Accepted:
                emit callerAccepted();
                break;
            case CommandServer::Busy:
                emit callerBusy();
                break;
            case CommandServer::UnknownCommand:
                 emit commandError();
                break;
            default:
                break;
        }
    }
}

quint16 CommandClient::generateUDPPort() {
    //num between 0 & 1.
    double rnd = qrand() / static_cast<double> (RAND_MAX);

   quint16 port = UDPMIN + (rnd *(UDPMAX - UDPMIN));

   qDebug() << "Generated Port" << port;
   return port;
}
