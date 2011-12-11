#include "soundreciever.h"

SoundReciever::SoundReciever(QObject *parent) :
    QBuffer(parent) {

        //create our udp socket and use the signal/slot mechanism to "Listen".
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(45454, QUdpSocket::ShareAddress);

        connect(udpSocket, SIGNAL(readyRead()),this,SLOT(processDatagrams()));
}

qint64 SoundReciever::readData ( char * data, qint64 len ) {

    //reset the position of the buffer to the start
    if(!seek(0)) {
        qDebug("Error resetting position of buffer: aborting");
        exit(1);
    }

    qDebug("Reading from buf request: %d",len);

    //Call to super to read the data
    qint64 result = QBuffer::readData(data,len);

    qDebug("buffer size : %d",buffer().size());
    //Our data has been read into the data variable, we can now remove it.
    buffer().remove(0,buffer().size());

    if (result != 0) {
        qDebug("Successfully read %d byes : %d", result);
    }

    return result;

}

void SoundReciever::processDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {

         //put a new array on the heap, this will be cleaned up when we detroy the buffer..
         QByteArray rawDatagram;
         rawDatagram.resize(udpSocket->pendingDatagramSize());
         udpSocket->readDatagram(rawDatagram.data(), rawDatagram.size());


         QByteArray decodedDatagram = qUncompress(rawDatagram);

         qDebug("Received %d bytes",rawDatagram.size());

         qint16 num = writeData(decodedDatagram.constData(),decodedDatagram.size());

         if (num == -1) {
             qDebug("Error! could not write data to receiver buffer");
         }
         else {
             //Documentaiton from QIODevice::readyRead states
             /*"Note for developers implementing classes derived from QIODevice:
                you should always emit readyRead() when new data has arrived"*/
             emit readyRead();

             qDebug("Data received, we wrote %d bytes to the buffer",num);
         }
         //checksum our data for debugging
         //quint16 val = qChecksum(datagram->constData(),datagram->size());
    }
}
