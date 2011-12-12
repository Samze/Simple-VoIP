#ifndef NETWORKDISCOVER_H
#define NETWORKDISCOVER_H

#include <QUdpSocket>
#include <QList>
#include <QString>
#include <QTimer>
#include <QNetworkInterface>
#include <QProcessEnvironment>
#include <QProcess>


class NetworkDiscover : public QUdpSocket
{
    Q_OBJECT
public:
    explicit NetworkDiscover(QObject *parent = 0);
    QTimer timer;

signals:
    void clientList(QList<QString*>*);

public slots:
    void receivedP2P();
    void notifyP2P();

private:
    static const int DISCOVER_PORT = 45453;
    static const int BROADCAST_TIME = 2000;
    QList<QString*> *clients;
    QNetworkInterface networkInter;
    QList<QHostAddress> localAddressList;
    QString broadCastMsg;

    bool isLocalAddress(QHostAddress);
    QString generatedBroadcastMsg();

};

#endif // NETWORKDISCOVER_H
