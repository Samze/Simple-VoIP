#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#include <QObject>
#include <QTcpSocket>

class CommandClient : public QObject
{
    Q_OBJECT
public:
    explicit CommandClient(QObject *parent = 0);

signals:
    void connectionEstablished();

public slots:
    void callUser(QString*,qint64);
    void hangUp();

private:
    QTcpSocket *m_tcpSocket;
    QString *m_ip;
    qint64 m_port;

};

#endif // COMMANDCLIENT_H
