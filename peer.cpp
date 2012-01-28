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



