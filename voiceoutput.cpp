#include "voiceoutput.h"

VoiceOutput::VoiceOutput() {

}

void VoiceOutput::playSound() {

    qDebug("Received signal, playing");

    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);


    format.setCodec("audio/pcm"); //according to the docs this is the codec supported by all platforms.
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    //By default we'll get the platforms default device
    devInfo = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());

    //Check support for format
    if (!devInfo->isFormatSupported(format)) {
        //TODO add error message here
        qWarning()<<"default format not supported try to use nearest";
        format = devInfo->nearestFormat(format);
    }


    audOut = new QAudioOutput(format,this);

    connect(audOut,SIGNAL(stateChanged(QAudio::State)),this, SLOT(finishedPlaying(QAudio::State)));

    //Checksum before play
    //quint16 val = qChecksum(buffer->data().constData(),buffer->size());
    //qDebug("chk before play : %d",val);

    soundRec = new SoundReciever();
    connect(soundRec,SIGNAL(readyRead()),this, SLOT(dataInBuffer()));

}

void VoiceOutput::finishedPlaying(QAudio::State state) {

    if (state == QAudio::ActiveState) {
        qDebug("Active state");
    }
    else if (state == QAudio::IdleState) {
        audOut->stop();
       // soundRec->close();
        delete soundRec;
        delete audOut;
    }
    else if (state == QAudio::StoppedState) {
        if (audOut->error() != QAudio::NoError) {
            // Error handling
           qint8 error = audOut->error();
           qDebug("Problems opening dev");
        } else {

        }
    }
}

void VoiceOutput::dataInBuffer() {
    //qDebug("Written bytes signal emitte");// %d", noBytes);
    if (!soundRec->isOpen()) {
        qDebug("Opening buffer");
        soundRec->open(QIODevice::ReadOnly);
        audOut->start(soundRec);
    }
}
