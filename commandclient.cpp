#include "commandclient.h"

CommandClient::CommandClient(QObject *parent) :
    QTcpSocket(parent) {

    connect(this,SIGNAL(readyRead()), this, SLOT(serverResponse()));
    connect(this, SIGNAL(connected()), this,SLOT(sendRequest()));
}

void CommandClient::callUser() {
    connectToHost("127.0.0.1",12345,QIODevice::ReadWrite);
}

void CommandClient::hangUp() {
    char command = static_cast<char>(CommandClient::EndCall);
    write(&command,1);
}

void CommandClient::sendRequest() {
    char command = static_cast<char>(CommandClient::Call);
    write(&command,1);
}

void CommandClient::serverResponse() {

    QByteArray input = readLine();


    //check input
    if (input.size() == 1) {
        CommandServer::ServerResponses response = static_cast<CommandServer::ServerResponses>(*input.data());

        switch(response) {
            case CommandServer::UnknownCommand:
                emit commandError();
                break;
            case CommandServer::Busy:
                emit callerBusy();
                break;
            default:
                break;
        }
    }
}
