#ifndef PEER_H
#define PEER_H

#include <QObject>
#include <QHostAddress>
#include <QTime>
#include <QHash>
#include <QStringList>

/**
  * Holds information for a located communication peer on the network. A Peer is someone on the same network who is also broad
  * casting to discover other users of this application. This class represents their username and address.
  *
  */
class Peer : public QObject
{
    Q_OBJECT
public:

    /**
      * Constructs a new peer.
      *@param parent Parent object.
      *@param user The username of the peer.
      *@param addr The IP of the peer.
      *@param port The prefered voice chat udp port for the peer. This is NOT implemented yet.
      */
    explicit Peer(QObject* parent,QString* user, QHostAddress* addr, quint16 port);
    ~Peer();

    /**
      * Static method that obtains a list of usernames from a list of peers.
      *@param peerList The list of peers.
      *@return A Stringlist containing all usernames of peers.
      */
    static QStringList getPeersNameList(const QList<Peer*> &peerList);

    /**
      * Returns the username of peer.
      *@return The username of the peer.
      */
    QString getName() const { return *username;}

    /**
      * Gets the time the peer last communicated with us.
      * @return The time of last communication.
      */
    QTime getLastResponded() { return time;}

    /**
      * Returns the IP address of the peer.
      * @return The IP address.
      */
    QHostAddress* getAddress() { return address;}

    /**
      * Updates the peers last communication time with the current local system time.
      */
    void updateTime();

private:
    QString * username; /**< username of peer. */
    QHostAddress* address;  /**< IP address of peer. */
    quint16 udpPort; /**< Prefered UDP port of peer (NOT USED YET( . */
    QTime time; /**< Time of last communication with peer. */

public:
    /**
      * Compares two peers by their username.
      */
    //This works but does not get called by Set "contains"...for whatever reason.
    bool operator==(const Peer &peer);

};

/**
  * Overloaded qHash in order to get Peer to contain in a Set.
  */
inline uint qHash(const Peer *peer) {
    uint hash = qHash(peer->getName());
    return hash;
}


/**
  * Overloaded obj1==obj2 in order to get Peer to contain in a Set.
  */
inline  bool operator==(const Peer &peer, const Peer &peer2) {
    return peer.getName() == peer2.getName();
}


#endif // PEER_H
