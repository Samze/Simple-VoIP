#include "voiceoutput.h"

VoiceOutput::VoiceOutput() {

    //Create our custom buffer, setting up a signal/slot for when this buffer starts filling up.
    m_soundReceiver = new SoundReciever();
    connect(m_soundReceiver,SIGNAL(readyRead()),this, SLOT(dataInBuffer()));

    m_format.setFrequency(44100);
    m_format.setChannels(2);
    m_format.setSampleSize(16);

    m_format.setCodec("audio/pcm"); //according to the docs this is the codec supported by all platforms.
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    //By default we'll get the platforms default device
    m_devInfo = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());

    //Check support for format
    if (!m_devInfo->isFormatSupported(m_format)) {
        //TODO add error message here
        qWarning()<<"default format not supported try to use nearest";
        m_format = m_devInfo->nearestFormat(m_format);
    }
}

VoiceOutput::~VoiceOutput() {
    delete m_soundReceiver;
    delete m_devInfo;
    //m_audioDevice gets created/deleted on play/stop
}

void VoiceOutput::playSound() {

    //setup our AudioOutput device and create a connet for it's state.
    m_audioOut = new QAudioOutput(m_format,this);
    connect(m_audioOut,SIGNAL(stateChanged(QAudio::State)),this, SLOT(finishedPlaying(QAudio::State)));


}

void VoiceOutput::finishedPlaying(QAudio::State state) {

    if (state == QAudio::ActiveState) {
        qDebug("Active state");
    }
    else if (state == QAudio::IdleState) { //Not sure this is a good idea.
        m_audioOut->stop();
        m_soundReceiver->close();
        delete m_soundReceiver;
        delete m_audioOut;
    }
    else if (state == QAudio::StoppedState) {
        if (m_audioOut->error() != QAudio::NoError) {
           qDebug("Error - problems opening dev : exiting");
           exit(1);
        }
    }
}

void VoiceOutput::dataInBuffer() {
    qDebug("Written bytes signal emitte");// %d", noBytes);
    if (!m_soundReceiver->isOpen()){// && m_soundReceiver->data().size() > 320) {
        qDebug("Opening buffer");
        m_soundReceiver->open(QIODevice::ReadOnly);
        m_audioOut->start(m_soundReceiver);
    }
}
