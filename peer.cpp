#include "peer.h"

Peer::Peer(QObject *parent) :
    QObject(parent)
{
}

Peer::~Peer() {
    delete m_username;
    delete m_address;
}


Peer::Peer(QObject *parent,QString* username, QHostAddress* address, quint16 udpPort)
    : QObject(parent), m_username(username), m_address(address), m_udpPort(udpPort){

    updateTime();

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



