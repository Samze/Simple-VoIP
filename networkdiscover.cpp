#include "networkdiscover.h"

NetworkDiscover::NetworkDiscover(QObject *parent) :
    QUdpSocket(parent)
{
    username = QProcessEnvironment::systemEnvironment().value("USERNAME");

    //Get a list of all local network addresses.
    localAddressList = networkInter.allAddresses();

    bind(NetworkDiscover::DISCOVER_PORT, QUdpSocket::ShareAddress);
    connect(this, SIGNAL(readyRead()),this,SLOT(receivedP2P()));

    broadCastTimer.setInterval(BROADCAST_TIME);
    connect(&broadCastTimer,SIGNAL(timeout()),this,SLOT(notifyP2P()));

    peerCheck.setInterval(CHECK_TIME);
    connect(&peerCheck,SIGNAL(timeout()),this,SLOT(checkPeersAlive()));

}
NetworkDiscover::~NetworkDiscover() {

}

void NetworkDiscover::receivedP2P() {
    while (hasPendingDatagrams()) {

        QHostAddress* senderAddress = new QHostAddress();
        quint16 senderPort;

        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        readDatagram(datagram.data(), datagram.size(),senderAddress,&senderPort);

        QString broadcastSig = datagram.left(6);

        if (broadcastSig == NETWORK_SIG) {
            if (1){//!isLocalAddress(senderAddress)) {

                QString *username = new QString(datagram.mid(6));
                bool added= addToList(username,senderAddress,0);

                if (!added) {
                    qDebug() << "Already contained";
                }

            }
        }
    }

}


bool NetworkDiscover::addToList(QString* senderName,QHostAddress* senderAddress,quint16 port) {

    Peer *peer = new Peer(this,senderName,senderAddress, port);

    qDebug() << *senderAddress << " : " << port << " | " << *senderName;

    if (peerList.contains(peer->getName())) {

        //Update time of response
        Peer* retrievedPeer = peerList.value(peer->getName());
        retrievedPeer->updateTime();
        delete peer;
    }
    else {
        peerList.insert(peer->getName(),peer);
    }

}

void NetworkDiscover::notifyP2P() {

   //TODO Check that I can get this environmental variable in LINUX/MACOS.

   QString test(NETWORK_SIG + username);
   QByteArray msg(test.toAscii());

   qDebug(msg);
   writeDatagram(msg,QHostAddress::Broadcast,NetworkDiscover::DISCOVER_PORT);
   //delete test;
}

bool NetworkDiscover::isLocalAddress(const QHostAddress senderAddress) {

    foreach(QHostAddress address, localAddressList) {

        if(address == senderAddress)
            return true;
    }
    return false;
}

void NetworkDiscover::checkPeersAlive() {

    QList<QString> toRemove;

    QMapIterator<QString, Peer*> i(peerList);
     while (i.hasNext()) {
         i.next();

         QTime peerTime = i.value()->getTime();
         qDebug() << peerTime.elapsed();

         if (peerTime.elapsed() > DISCOVER_TIMEOUT ) {
            toRemove.append(i.key());
         }
     }


     foreach(QString id, toRemove) {
         peerList.remove(id);
     }

     emit peersChanged(peerList.values());
}
