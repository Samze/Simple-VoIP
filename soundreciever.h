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
	
#ifndef SOUNDRECIEVER_H
#define SOUNDRECIEVER_H

#include <QUdpSocket>
#include <QBuffer>
//#include "ogg/ogg.h"

/**
  * A buffer that receives UDP audio data over the network. This class listens on a specific UDP port for audio data
  * sent by another client. It decompresses it and adds it to it's buffer. Read requests on this buffer will empty it.
  * This class inherits from QBuffer.
  */
class SoundReciever : public QBuffer
{
    Q_OBJECT
public:

    /**
      *Default constructor which binds to a UDP port.
      *@param parent The owner of this object.
      */
    explicit SoundReciever(QObject *parent = 0);

    /**
      * Deletes the UDPSocket.
      */
    ~SoundReciever();

protected:
    /**
      * Overridden QBuffer method. This method will empty the buffer on a read request.
      * @param data A pointer to data where it should be placed.
      * @param len The length of data to try and read
      * @return The length actually read into data.
      * @see QBuffer::readData()
      */
    qint64 readData ( char * data, qint64 len );

private slots:
    /**
      * Called everytime new data is received. This method will decompress the data and add it to our buffer.
      * @see QBuffer::buffer()
      */
    void processDatagrams();

private:
    QUdpSocket *udpSocket; /**< The UDP socket used. */

};

#endif // SOUNDRECIEVER_H
