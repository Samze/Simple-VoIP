#include "networkdiscover.h"

NetworkDiscover::NetworkDiscover(QObject *parent) :
    QUdpSocket(parent)
{

  bind(NetworkDiscover::DISCOVER_PORT, QUdpSocket::ShareAddress);

  connect(this, SIGNAL(readyRead()),this,SLOT(receivedP2P()));

  timer.setInterval(BROADCAST_TIME);

  connect(&timer,SIGNAL(timeout()),this,SLOT(timer()));
}


void NetworkDiscover::receivedP2P() {
    while (hasPendingDatagrams()) {

         QByteArray datagram;
         datagram.resize(pendingDatagramSize());
         readDatagram(datagram.data(), datagram.size());

         if (datagram != "Sam") {
             qDebug() << datagram;
         }
    }
}


void NetworkDiscover::notifyP2P() {

    QByteArray notifyBody("Sam");

    writeDatagram(notifyBody,QHostAddress::Broadcast,NetworkDiscover::DISCOVER_PORT);
}
