#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class CommandServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CommandServer(QObject *parent = 0);


private slots:
    void receiveConnection();
    void readRequest();

};

#endif // COMMANDSERVER_H
