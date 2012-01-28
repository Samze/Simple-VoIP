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
	
#include "voiceinput.h"

VoiceInput::VoiceInput(QHostAddress address)
{
    qDebug() << address;
    //setup audio
    audioIn = new QAudioInput(format, this);
    //init buffer
    buffer = new SoundSender(this,address);
}

VoiceInput::~VoiceInput() {  
    delete audioIn;
    //m_audioDevice gets created/deleted on play/stop
}

void VoiceInput::start() {
    connect(audioIn,SIGNAL(stateChanged(QAudio::State)),this,SLOT(audioState(QAudio::State)));

    buffer->open(QIODevice::WriteOnly);

   // m_audioIn->setBufferSize(4096);
    audioIn->start(buffer);
}

void VoiceInput::audioState(QAudio::State state) {

    if (state == QAudio::ActiveState) {
        qDebug("Input device actived");
    }
    else if (state == QAudio::IdleState) {
        qDebug("Input went idle, ££££");
    }
    else if (state == QAudio::StoppedState) {
        if (audioIn->error() != QAudio::NoError) {
           // Error handling.
           qDebug("Problems with input device");
        }
    }
}

void VoiceInput::stop() {

    if (buffer->isOpen()) {
        buffer->close();
    }
    if (audioIn->state() == QAudio::ActiveState || audioIn->state() == QAudio::IdleState) {
        audioIn->stop();
    }
}


void VoiceInput::muteMic(bool toggle) {
    if (audioIn->state() == QAudio::ActiveState || audioIn->state() == QAudio::IdleState) {
        if (toggle) {
            audioIn->suspend();
        }
        else {
            audioIn->resume();
        }
    }
}


