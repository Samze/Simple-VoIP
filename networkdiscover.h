#ifndef NETWORKDISCOVER_H
#define NETWORKDISCOVER_H

#include <QUdpSocket>
#include <QList>
#include <QSet>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QNetworkInterface>
#include <QProcessEnvironment>
#include <QProcess>
#include "peer.h"

#define NETWORK_SIG "@SGv1#"
#define DISCOVER_TIMEOUT 1000

class NetworkDiscover : public QUdpSocket
{
    Q_OBJECT
public:
    explicit NetworkDiscover(QObject *parent = 0);
    ~NetworkDiscover();
    QTimer broadCastTimer;
    QTimer peerCheck;
    QMap<QString,Peer*> peerList;

signals:
    void peersChanged(QList<Peer*>);
public slots:
    void receivedP2P();
    void notifyP2P();
    void checkPeersAlive();

private:
    static const int DISCOVER_PORT = 45453;
    static const int BROADCAST_TIME = 2000;
    static const int CHECK_TIME = 10000;
    //static const QString BroadcastHeader = "#SGv1#";
   // QSet<Peer*> peerList; //Can't seem to get it to call the == overloaded operator properly


    QNetworkInterface networkInter;
    QList<QHostAddress> localAddressList;
    QString username;

    bool isLocalAddress(QHostAddress);
    bool addToList(QString*,QHostAddress*,quint16);

};

#endif // NETWORKDISCOVER_H
