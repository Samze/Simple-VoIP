#include "captureaudio.h"

CaptureAudio::CaptureAudio()
{
    m_soundSender = new SoundSender();

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
}

CaptureAudio::~CaptureAudio() {
    delete m_devInfo;
    delete m_soundSender;
    //m_audioDevice gets created/deleted on play/stop
}

void CaptureAudio::recordSound() {

    //setup audio
    m_audioIn = new QAudioInput(m_format, this);

    connect(m_audioIn,SIGNAL(stateChanged(QAudio::State)),this,SLOT(audioStateSlot(QAudio::State)));

    m_soundSender->open(QIODevice::WriteOnly);

   // m_audioIn->setBufferSize(4096);
    m_audioIn->start(m_soundSender);
}

void CaptureAudio::audioStateSlot(QAudio::State state) {

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

void CaptureAudio::stopRecording() {

    m_audioIn->stop();
    delete m_audioIn;
}


