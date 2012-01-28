/*	
    VoIPApp- A simple p2p voip application written in C++ using the QT Framework.
    Copyright (C) 2011  Sam Gunaratne University of Plymouth

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
	
#include "networkdiscover.h"

NetworkDiscover::NetworkDiscover(QObject *parent) :
    QUdpSocket(parent)
{
    QString name;

    //We want to obtain a username automatically. We do this by accessing system variables. Sadly
    //different platforms use different things.
    #ifdef Q_WS_MAC
    name = "USER";
    #endif

    #ifdef Q_WS_WIN
    name = "USERNAME";
    #endif

    #ifdef Q_WS_X11
    name = "USERNAME";
    #endif

    username = QProcessEnvironment::systemEnvironment().value(name);

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
            //Change this setting to stop us being able to add ourselves.
            if (1){//!isLocalAddress(senderAddress)) {

                QString *username = new QString(datagram.mid(6));
                addToList(username,senderAddress,0);

            }
        }
    }

}

//NOTE port is not supported yet, it is for an enhancement where the communication port can be varied.
bool NetworkDiscover::addToList(QString* senderName,QHostAddress* senderAddress,quint16 port) {

    Peer *peer = new Peer(this,senderName,senderAddress, port);

    qDebug() << "Peer: " << *senderAddress << " : " << port << " | " << *senderName;

    if (peerList.contains(peer->getName())) {

        //Update time of response
        Peer* retrievedPeer = peerList.value(peer->getName());
        retrievedPeer->updateTime();
        delete peer;
        return false;
    }
    else {
        peerList.insert(peer->getName(),peer);
        emit peersChanged(peerList.values());
        return true;
    }

}

void NetworkDiscover::notifyP2P() {

   QString test(NETWORK_SIG + username);
   QByteArray msg(test.toAscii());

   writeDatagram(msg,QHostAddress::Broadcast,NetworkDiscover::DISCOVER_PORT);
}

bool NetworkDiscover::isLocalAddress(const QHostAddress senderAddress) {

    foreach(QHostAddress address, localAddressList) {

        if(address == senderAddress)
            return true;
    }
    return false;
}

void NetworkDiscover::checkPeersAlive() {

    //Create an empty list to store out timed out peers
    QList<QString> toRemove;

    //Iterate through checking that their last response time isn't too long ago
    QMapIterator<QString, Peer*> i(peerList);
     while (i.hasNext()) {
         i.next();

         QTime peerTime = i.value()->getLastResponded();
         qDebug() << peerTime.elapsed();

         if (peerTime.elapsed() > DISCOVER_TIMEOUT ) {
            toRemove.append(i.key());
         }
     }

     //remove all timed out peers
     foreach(QString id, toRemove) {
         peerList.remove(id);
     }

     //If we removed any, emit a signal
     if (toRemove.size() > 0) {
         emit peersChanged(peerList.values());
     }
}


Peer* NetworkDiscover::getPeerFromAddress(const QHostAddress &address) {

    foreach(Peer* p, peerList.values()) {
        if (*p->getAddress() == address) {
            return p;
        }
    }
    return NULL;
}

bool NetworkDiscover::startTimers() {
    broadCastTimer.start();
    peerCheck.start();

    return peerCheck.isActive() && broadCastTimer.isActive();
}
