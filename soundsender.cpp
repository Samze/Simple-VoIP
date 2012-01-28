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
	
#include "soundsender.h"

SoundSender::SoundSender(QObject *parent, QHostAddress addr) :
    QBuffer(parent), address(addr)
{
    udpSocket = new QUdpSocket(this);
}

SoundSender::~SoundSender() {
    delete udpSocket;
}

qint64 SoundSender::writeData(const char *data, qint64 len){

    QByteArray uncompressed(data,len);

    QByteArray compressed = qCompress(uncompressed,9);

    float compPercent = ((float)compressed.size()/len) * 100;
    qDebug() << "Compressed " << len << " bytes into " << compressed.size() << " giving " << 100 - compPercent << "% compression rate";

    if (compressed.size() > 0) {    //If we have data - send it!
        qDebug("Sending data : %d",compressed.size());

        udpSocket->writeDatagram(compressed.constData(),
                                 compressed.size(),
                                 address,
                                 45454);
    }

    return len;
}
