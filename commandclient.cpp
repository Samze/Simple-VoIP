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
    sendCommand(CommandClient::EndCall);
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

void CommandClient::sendCommand(CommandClient::CallCommand cmd) {
    char command = static_cast<char>(cmd);
    write(&command,1);
}


quint16 CommandClient::generateUDPPort() {
    //num between 0 & 1.
    double rnd = qrand() / static_cast<double> (RAND_MAX);

   quint16 port = UDPMIN + (rnd *(UDPMAX - UDPMIN));

   qDebug() << "Generated Port" << port;
   return port;
}

