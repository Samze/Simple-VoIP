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
	
#ifndef SOUNDSENDER_H
#define SOUNDSENDER_H

#include <QThread>
#include <QBuffer>
#include <QUdpSocket>
#include <QAudioFormat>
//#include "ogg/ogg.h"

/**
  * A buffer that sends UDP audio data over the network. This class sends audio data down a specific UDP port
  * to another client. When data is written to this buffer, it is immenditely compressed and sent down the network.
  */
class SoundSender : public QBuffer
{
    Q_OBJECT

public:

    /**
      *Default constructor which binds to a UDP port.
      *@param parent The owner of this object.
      *@param addr The address to send the data.
      */
    explicit SoundSender(QObject *parent, QHostAddress addr);

    /**
      * Deletes the UDPSocket.
      */
    ~SoundSender();

protected:
    /**
      * Overridden from QBuffer, when data is written to this buffer it is sent down the network to the
      * address specified.
      * @param data The data to put into the buffer.
      * @param len The length of data to add.
      * @return The length of data sent.
      */
    qint64 writeData (const char * data, qint64 len);

private:
    QUdpSocket *udpSocket; /**< The udp socket used. */
    QHostAddress address; /**< The address to send the data. */

//Was playing around using OGG transport layer and speex compression...lots of effort..
//    int packetNo;
//    int frameNo;
//    int pageNo;
//    ogg_page  og;
//    ogg_stream_state os;

};

#endif // SOUNDSENDER_H
