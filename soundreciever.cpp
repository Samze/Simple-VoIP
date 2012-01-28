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
	
#include "soundreciever.h"

SoundReciever::SoundReciever(QObject *parent) :
    QBuffer(parent) {

        //create our udp socket and use the signal/slot mechanism to "Listen".
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(45454, QUdpSocket::ShareAddress);

        connect(udpSocket, SIGNAL(readyRead()),this,SLOT(processDatagrams()));
}


SoundReciever::~SoundReciever() {
    delete udpSocket;
}

qint64 SoundReciever::readData ( char * data, qint64 len ) {

    //reset the position of the buffer to the start
    if(!seek(0)) {
        qWarning("Error resetting position of buffer: aborting");
    }

    //Call to super to read the data
    qint64 result = QBuffer::readData(data,len);

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
             qWarning("Error! could not write data to receiver buffer");
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
