#include "voiceoutput.h"

VoiceOutput::VoiceOutput(AbstractVoice *parent) :
    AbstractVoice(parent) {

    //Create our custom buffer, setting up a signal/slot for when this buffer starts filling up.
    buffer = new SoundReciever(this);

    //This may want to go in START
    //setup our AudioOutput device and create a connet for it's state.
    audioOut = new QAudioOutput(format,this);
    connect(audioOut,SIGNAL(stateChanged(QAudio::State)),this, SLOT(audioState(QAudio::State)));

    timedout = 0;
}

VoiceOutput::~VoiceOutput() {
    delete audioOut;
}

void VoiceOutput::start() {
    connect(buffer,SIGNAL(readyRead()),this, SLOT(dataInBuffer()));

   // m_audioOut->setBufferSize(4096);
}

void VoiceOutput::stop(){
    qDebug("Stopped : timed out %d times..",timedout);


    if (buffer->isOpen()) {
        buffer->close();
    }
    if (audioOut->state() == QAudio::ActiveState || audioOut->state() == QAudio::IdleState) {
        audioOut->stop();
    }

    disconnect(buffer,SIGNAL(readyRead()),0,0);
}

void VoiceOutput::audioState(QAudio::State state) {

    if (state == QAudio::ActiveState) {
        qDebug("Output device actived");
    }
    else if (state == QAudio::IdleState) {
        qDebug("erm, we went idle...mic muted probably");

        //This line stops all playback on MAC_OS
        #ifndef Q_WS_MAC
            audioOut->start(buffer);
        #endif

        ++timedout;
    }
    else if (state == QAudio::StoppedState) {
        if (audioOut->error() != QAudio::NoError) {
           qDebug("Error - problems opening dev : exiting");
           exit(1);
        } else {
            qDebug("We stopped ...for some reason (Muted)");
        }
    }
}

void VoiceOutput::dataInBuffer() {
    if (!buffer->isOpen() && buffer->bytesAvailable() > 10000){
        qDebug("Opening audio out buffer");
        buffer->open(QIODevice::ReadOnly);
        audioOut->start(buffer);
    }
}

void VoiceOutput::muteSound(bool toggle) {
    if (audioOut->state() == QAudio::ActiveState || audioOut->state() == QAudio::IdleState) {
        if (toggle) {
            audioOut->suspend();
        }
        else {
            buffer->buffer().clear();
            audioOut->resume();
        }
    }
}
