#include "peer.h"


Peer::Peer(QObject *parent,QString* name, QHostAddress* addr, quint16 udp)
    : QObject(parent), username(name), address(addr), udpPort(udp){

    //Set that we have heard from this user when we created it.
    updateTime();

}
Peer::~Peer() {
    delete username;
    delete address;
}


void Peer::updateTime() {
    time = QTime::currentTime();
}

bool Peer::operator==(const Peer &peer)
{
       return peer.getName() == getName();
}


QStringList Peer::getPeersNameList(const QList<Peer*> &peerList) {

    QStringList list;

    foreach(Peer* peer , peerList) {

        list.append(peer->getName());
    }
    return list;
}



