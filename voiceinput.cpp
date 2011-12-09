#include "VoiceInput.h"

VoiceInput::VoiceInput(QObject *parent) :
    QObject(parent)
{
    //configure our format.

    m_format.setFrequency(8000);
    m_format.setChannels(2);
    m_format.setSampleSize(16);

    m_format.setCodec("audio/pcm"); //according to the docs this is the codec supported by all platforms.
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    //By default we'll get the platforms default device
    m_devInfo = new QAudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice());


    //Check support for format
    if (!m_devInfo->isFormatSupported(m_format)) {
        //TODO add error message here
        qWarning()<<"default format not supported try to use nearest";
        m_format = m_devInfo->nearestFormat(m_format);
    }

    //setup audio
    m_audioIn = new QAudioInput(m_format, this);
    //init buffer
    m_soundSender = new SoundSender(this);
}

VoiceInput::~VoiceInput() {
    delete m_devInfo;  
    delete m_audioIn;
    delete m_soundSender;
    //m_audioDevice gets created/deleted on play/stop
}

void VoiceInput::recordSound() {
    connect(m_audioIn,SIGNAL(stateChanged(QAudio::State)),this,SLOT(audioStateSlot(QAudio::State)));

    m_soundSender->open(QIODevice::WriteOnly);

   // m_audioIn->setBufferSize(4096);
    m_audioIn->start(m_soundSender);
}

void VoiceInput::audioStateSlot(QAudio::State state) {

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

void VoiceInput::stopRecording() {

    if (m_soundSender->isOpen()) {
        m_soundSender->close();
    }
    if (m_audioIn->state() == QAudio::ActiveState || m_audioIn->state() == QAudio::IdleState) {
        m_audioIn->stop();
    }
}


