#include "networkdiscover.h"

NetworkDiscover::NetworkDiscover(QObject *parent) :
    QUdpSocket(parent)
{

  bind(NetworkDiscover::DISCOVER_PORT, QUdpSocket::ShareAddress);

  connect(this, SIGNAL(readyRead()),this,SLOT(receivedP2P()));
}


void NetworkDiscover::receivedP2P() {
    while (hasPendingDatagrams()) {

         QByteArray datagram;
         datagram.resize(pendingDatagramSize());
         readDatagram(datagram.data(), datagram.size());

         if (datagram != "sam") {
             qDebug() << datagram;
         }
    }
}


void NetworkDiscover::notifyP2P() {

    QByteArray notify("Sam");

    writeDatagram(notify,QHostAddress::Broadcast,NetworkDiscover::DISCOVER_PORT);
}
