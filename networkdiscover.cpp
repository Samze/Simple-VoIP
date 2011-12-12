#include "networkdiscover.h"

NetworkDiscover::NetworkDiscover(QObject *parent) :
    QUdpSocket(parent)
{
  broadCastMsg = generatedBroadcastMsg();
  //Get a list of all local network addresses.
  localAddressList = networkInter.allAddresses();

  bind(NetworkDiscover::DISCOVER_PORT, QUdpSocket::ShareAddress);
  connect(this, SIGNAL(readyRead()),this,SLOT(receivedP2P()));

  timer.setInterval(BROADCAST_TIME);
  connect(&timer,SIGNAL(timeout()),this,SLOT(notifyP2P()));
}


void NetworkDiscover::receivedP2P() {
    while (hasPendingDatagrams()) {

         QHostAddress senderAddress;
         quint16 senderPort;

         QByteArray datagram;
         datagram.resize(pendingDatagramSize());
         readDatagram(datagram.data(), datagram.size(),&senderAddress,&senderPort);

        // if (!isLocalAddress(senderAddress)) {

             QString username = datagram.mid(4);
             //TODO We don't want to accept any old udp packet, only ones with the matching datagram.
             qDebug() << senderAddress << " : " << senderPort <<
                      " | " << username;
      //   }
    }
}


void NetworkDiscover::notifyP2P() {

    QByteArray notifyBody(broadCastMsg.toAscii());

    qDebug(notifyBody);
    writeDatagram(notifyBody,QHostAddress::Broadcast,NetworkDiscover::DISCOVER_PORT);
}

bool NetworkDiscover::isLocalAddress(const QHostAddress senderAddress) {

    foreach(QHostAddress address, localAddressList) {

        if(address == senderAddress)
           return true;
    }
    return false;
}

QString NetworkDiscover::generatedBroadcastMsg() {

    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();

    //TODO Check that I can get this environmental variable in LINUX/MACOS.
    QString username =  environment.value("USERNAME");

    return QString("#SG#" + username);
}
