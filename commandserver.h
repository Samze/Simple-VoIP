#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class CommandServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CommandServer(QObject *parent = 0);

signals:
    void callInitiated();

private slots:
    void receiveConnection();
    void readRequest();

private:
    QTcpSocket *receivedSocket;
    QList<QTcpSocket *> *activeConnections;
};

#endif // COMMANDSERVER_H
