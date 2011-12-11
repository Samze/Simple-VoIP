#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "commandserver.h"


class CommandClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CommandClient(QObject *parent = 0);

    enum CallCommands {
       Call,
       EndCall,
       MuteMic,
       MuteSound
    };

signals:
    void connectionEstablished();
    void commandError();
    void callerBusy();

public slots:
    void callUser();
    void hangUp();
    void sendRequest();
    void serverResponse();

//    QString *m_ip;
//    qint64 m_port;

};

#endif // COMMANDCLIENT_H
