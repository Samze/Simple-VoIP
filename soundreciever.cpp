#include "soundreciever.h"

SoundReciever::SoundReciever(){
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(45454, QUdpSocket::ShareAddress);

        connect(udpSocket, SIGNAL(readyRead()),this,SLOT(processDatagrams()));
}

qint64 SoundReciever::readData ( char * data, qint64 len ) {
    bool sought = seek(0);

    //qDebug("Reading from buf request: %d",len);
   // qDebug("Buffer size on readData %d", buffer().size());

    //add artifical delay
    qint64 result = QBuffer::readData(data,len);

    /*QByteArray* byteA = new QByteArray(buffer().remove(0,buffer().size()));
    data = byteA->data();
    qint64 result = byteA->size()*/;

    buffer().remove(0,buffer().size());

    if (result != 0) {

        qDebug("Successfully read %d", result);
    }
   // qDebug("Buffer size AFTER read %d", buffer().size());

    return result;

}


void SoundReciever::processDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {

         //put a new array on the heap, this will be cleaned up when we detroy the buffer..
         QByteArray* datagram = new QByteArray();
         datagram->resize(udpSocket->pendingDatagramSize());
         udpSocket->readDatagram(datagram->data(), datagram->size());

         //open(QIODevice::ReadWrite);
         qint16 num = writeData(datagram->constData(),datagram->size());
         qDebug("Data received, we wrote %d bytes to the buffer",num);

         //checksum our data for debugging
         quint16 val = qChecksum(datagram->constData(),datagram->size());
         //qDebug("chk on receive : %d",val);

         //qDebug("RecBufferzz : %d,",buffer().size());


         //close();
    }
}
