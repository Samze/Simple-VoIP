#include "voiceoutput.h"

VoiceOutput::VoiceOutput(AbstractVoice *parent) :
    AbstractVoice(parent) {

    //Create our custom buffer, setting up a signal/slot for when this buffer starts filling up.
    buffer = new SoundReciever(this);
    connect(buffer,SIGNAL(readyRead()),this, SLOT(dataInBuffer()));

    timedout = 0;
}

VoiceOutput::~VoiceOutput() {

}

void VoiceOutput::start() {

    //setup our AudioOutput device and create a connet for it's state.
    m_audioOut = new QAudioOutput(format,this);
    connect(m_audioOut,SIGNAL(stateChanged(QAudio::State)),this, SLOT(audioState(QAudio::State)));

   // m_audioOut->setBufferSize(4096);

}

void VoiceOutput::stop(){
    qDebug("Stopped : timed out %d times..",timedout);
}

void VoiceOutput::audioState(QAudio::State state) {

    if (state == QAudio::ActiveState) {
        qDebug("Output device actived");
    }
    else if (state == QAudio::IdleState) {
        qDebug("erm, we went idle...nooo");
        m_audioOut->start(buffer);
        ++timedout;
        //TODO add something to timeout a user..
    }
    else if (state == QAudio::StoppedState) {
        if (m_audioOut->error() != QAudio::NoError) {
           qDebug("Error - problems opening dev : exiting");
           exit(1);
        } else {
        qDebug("We stopped ...for some reason");
        }
    }
}

void VoiceOutput::dataInBuffer() {
    if (!buffer->isOpen() && buffer->bytesAvailable() > 10000){
        qDebug("Opening audio out buffer");
        buffer->open(QIODevice::ReadOnly);
        m_audioOut->start(buffer);
    }
}
