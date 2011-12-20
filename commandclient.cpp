#include "commandclient.h"

CommandClient::CommandClient(QObject *parent) :
    QTcpSocket(parent) {

    connect(this,SIGNAL(readyRead()), this, SLOT(serverResponse()));
    connect(this, SIGNAL(connected()), this,SLOT(callPeer()));
}

void CommandClient::connectToPeer(Peer* peer) {

    connectToHost(*peer->getAddress(),12345,QIODevice::ReadWrite);
}

void CommandClient::hangUp() {
    char command = static_cast<char>(CommandClient::EndCall);
    write(&command,1);
    disconnectFromHost();
}

void CommandClient::callPeer() {
    char command = static_cast<char>(CommandClient::Call);

    write(&command,1);
}

void CommandClient::serverResponse() {

    QByteArray input = readLine();

    //check input
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
