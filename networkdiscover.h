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

/** Signatue that is used as a header for the discovery process */
#define NETWORK_SIG "@SGv1#"

/**
 * Discovers local peers on the network to communicate with. This class broadcasts and receives a beacon signal that
 * is used to determine who is avaiable on the network for calling. A signal is sent out periodically to show an "alive"
 * state. The same port is binded to listen to other peers, they are added to a list. Periodically a check is done to see
 * the last know time of an update. This is used to remove peers from the list. This class inherits from QUdpSocket.
 *@see QUdpSocket
 */
class NetworkDiscover : public QUdpSocket
{
    Q_OBJECT
public:
    /**
      * Gets local name, binds udp port, connects signals and slots and then starts timers.
      * @param parent Parent object.
      */
    explicit NetworkDiscover(QObject *parent = 0);

    /**
      * Default destructor.
      */
    ~NetworkDiscover();

    /**
      * Returns a peer for an address.
      *@param address The address for the peer.
      *@return The returned peer or null.
      */
    Peer* getPeerFromAddress(const QHostAddress &address);

    /**
      *Starts the peer discovery timers. Starts the peer alive check timer and the broadcast timer.
      *@return Returns if timers were started successfully.
      */
    bool startTimers();

    /**
      * The list of peers currently detected on the network.
      */
    //NOTE this should really be a QSet<Peer*>. However after much frustration I could never get it to properly
    //call the overloaded == operator properly when adding to a set. The implementation of == and qHash seemed ok.
    //The work around is to use a map with a String key and Peer value. This is not ideal as I wanted to combine IP
    //and also username to form the unique identifier.
    QMap<QString,Peer*> peerList;

signals:
    void peersChanged(QList<Peer*>); /**< Emitted when the peer list has changed, ie. new peer or removed peer.*/

private slots:
    /**
      * Received peer signal from UDP port. Check its id against our maintained list.
      */
    void receivedP2P();

    /**
      * Broadcasts an identifier beacon via UDP. The broadcast is done periodically on a timer.
      */
    void notifyP2P();

    /**
      * Periodically checks the last time a peer replied to our signal. If it is more than a threshold, they are removed.
      */
    void checkPeersAlive();

private:
    //These could be #DEFINE however static const respects scope and is type-safe.
    static const int DISCOVER_PORT = 45453;     /**< The UDP port to be used for discovery*/
    static const int BROADCAST_TIME = 2000;     /**< The time between broadcasting our signal in ms.*/
    static const int CHECK_TIME = 10000;        /**< The time between checking our peers are still "alive"*/
    static const int DISCOVER_TIMEOUT = 3000;   /**< The time within that a peer should have last communicated.*/

    QTimer broadCastTimer; /**< Timer used to periodically broadcast that we are an alive peer. */
    QTimer peerCheck; /**< Timer used to periodically check that our peers are still active. */

    QNetworkInterface networkInter; /**< Used to obtain local network information. */
    QList<QHostAddress> localAddressList; /**< Local address list */

    QString username; /**< Local username detected. */

private:
    /**
      * Detects if the address is local. This method can be called to see if the broadcast message is actually us, we
      * do this by checking all our local addresses against the one received.
      */
    bool isLocalAddress(QHostAddress);

    /**
      * Adds a peer to the list, with their name, address and their prefered communication port. Preferred port is not yet supported.
      */
    bool addToList(QString*,QHostAddress*,quint16);

};

#endif // NETWORKDISCOVER_H
