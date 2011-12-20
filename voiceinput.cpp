#include "voiceinput.h"

VoiceInput::VoiceInput(QHostAddress address)
{
    qDebug() << address;
    //setup audio
    m_audioIn = new QAudioInput(format, this);
    //init buffer
    buffer = new SoundSender(this,address);
}

VoiceInput::~VoiceInput() {  
    delete m_audioIn;
    //m_audioDevice gets created/deleted on play/stop
}

void VoiceInput::start() {
    connect(m_audioIn,SIGNAL(stateChanged(QAudio::State)),this,SLOT(audioState(QAudio::State)));

    buffer->open(QIODevice::WriteOnly);

   // m_audioIn->setBufferSize(4096);
    m_audioIn->start(buffer);
}

void VoiceInput::audioState(QAudio::State state) {

    if (state == QAudio::ActiveState) {
        qDebug("Input device actived");
    }
    else if (state == QAudio::IdleState) {
        qDebug("Input went idle, ££££");
    }
    else if (state == QAudio::StoppedState) {
        if (m_audioIn->error() != QAudio::NoError) {
           // Error handling.
           qDebug("Problems with input device");
        }
    }
}

void VoiceInput::stop() {

    if (buffer->isOpen()) {
        buffer->close();
    }
    if (m_audioIn->state() == QAudio::ActiveState || m_audioIn->state() == QAudio::IdleState) {
        m_audioIn->stop();
    }
}


void VoiceInput::muteMic(bool toggle) {
    if (toggle) {
        m_audioIn->suspend();
    }
    else {
        m_audioIn->resume();
    }
}


