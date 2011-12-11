#ifndef NETWORKDISCOVER_H
#define NETWORKDISCOVER_H

#include <QUdpSocket>
#include <QList>
#include <QString>

class NetworkDiscover : public QUdpSocket
{
    Q_OBJECT
public:
    explicit NetworkDiscover(QObject *parent = 0);

signals:
    void clientList(QList<QString*>*);

public slots:
    void receivedP2P();
    void notifyP2P();

private:
    static const int DISCOVER_PORT = 45453;
    QList<QString*> *clients;
};

#endif // NETWORKDISCOVER_H
