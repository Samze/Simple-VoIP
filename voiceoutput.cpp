#include "voiceoutput.h"

VoiceOutput::VoiceOutput(AbstractVoice *parent) :
    AbstractVoice(parent) {

    //Create our custom buffer, setting up a signal/slot for when this buffer starts filling up.
    buffer = new SoundReciever(this);
    connect(buffer,SIGNAL(readyRead()),this, SLOT(dataInBuffer()));

    timedout = 0;
}

VoiceOutput::~VoiceOutput() {
    delete m_audioOut;
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
        qDebug("erm, we went idle...mic muted probably");
        m_audioOut->start(buffer);
        ++timedout;
    }
    else if (state == QAudio::StoppedState) {
        if (m_audioOut->error() != QAudio::NoError) {
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
        m_audioOut->start(buffer);
    }
}

void VoiceOutput::muteSound(bool toggle) {
    qDebug() << "MUTED sound!";
    if (toggle) {
        m_audioOut->suspend();
    }
    else {
        buffer->buffer().clear();
        m_audioOut->resume();

    }
}
