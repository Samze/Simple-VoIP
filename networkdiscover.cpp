#include "networkdiscover.h"

NetworkDiscover::NetworkDiscover(QObject *parent) :
    QUdpSocket(parent)
{


  bind(NetworkDiscover::DISCOVER_PORT, QUdpSocket::ShareAddress);
  connect(this, SIGNAL(readyRead()),this,SLOT(receivedP2P()));

  timer.setInterval(BROADCAST_TIME);
  connect(&timer,SIGNAL(timeout()),this,SLOT(notifyP2P()));
}


void NetworkDiscover::receivedP2P() {
    while (hasPendingDatagrams()) {

         QByteArray datagram;
         datagram.resize(pendingDatagramSize());
         readDatagram(datagram.data(), datagram.size());

         if (datagram != "Sam") {
             //TODO We don't want to accept any old udp packet, only ones with the matching datagram.
             qDebug() << peerAddress().toString() << " | " << datagram;
         }
    }
}


void NetworkDiscover::notifyP2P() {

    QByteArray notifyBody("Hello World!");

    writeDatagram(notifyBody,QHostAddress::Broadcast,NetworkDiscover::DISCOVER_PORT);
}
