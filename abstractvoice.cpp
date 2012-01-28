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
	
#include "abstractvoice.h"

AbstractVoice::AbstractVoice(QObject *parent) : QObject(parent)
{
    //This is the default format which works locally with no special codec/advanced compression.
    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);

    format.setCodec("audio/pcm"); //according to the docs this is the codec supported by all platforms.
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    //By default we'll get the platforms default device, could improve this and let the user pick later.
    devInfo = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());

    //Check support for format
    if (!devInfo->isFormatSupported(format)) {
        qWarning()<<"default format not supported, trying to use nearest";
        format = devInfo->nearestFormat(format);
    }
}

AbstractVoice::~AbstractVoice () {
    delete devInfo;
    delete buffer;
}
