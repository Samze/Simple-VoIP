#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "commandclient.h"

class CommandServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CommandServer(QObject *parent = 0);
    ~CommandServer();


    enum ServerResponses {
       UnknownCommand,
       Busy
    };

signals:
    void callInitiated(QHostAddress);
    void callEnded();
    void micMuted();
    void soundMuted();

private slots:
    void receiveConnection();
    void readRequest();

private:
    QList<QTcpSocket *> *activeConnections;
};

#endif // COMMANDSERVER_H
