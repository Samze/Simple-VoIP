#include "captureaudio.h"

CaptureAudio::CaptureAudio()
{
    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm"); //according to the docs this is the codec supported by all platforms.
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    //By default we'll get the platforms default device
    devInfo = new QAudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice());

    //Check support for format
    if (!devInfo->isFormatSupported(format)) {
        //TODO add error message here
        qWarning()<<"default format not supported try to use nearest";
        format = devInfo->nearestFormat(format);
    }

    buffer = new QBuffer(this);

}

CaptureAudio::~CaptureAudio() {
    delete audioDev;
    delete devInfo;
    delete buffer;
}

void CaptureAudio::recordSound() {
    buffer->open(QIODevice::WriteOnly);
    //setup audio
    audioDev = new QAudioInput(format, this);
    audioDev->start(buffer);
}


void CaptureAudio::stopRecording() {
    audioDev->stop();
    delete audioDev;
    buffer->close();

    buffer->open(QIODevice::ReadOnly);
    qDebug("Bytes available = %ld",buffer->bytesAvailable());


    QByteArray array = buffer->data();

    for (int i=0; i < array.size(); ++i) {
       int chars = static_cast<int>(array.at(i));
        qDebug("%d",chars);
    }

    //send data, play sound
    emit hasVoiceData(buffer);
}

void CaptureAudio::playSound(QBuffer *buf ) {
    audOut = new QAudioOutput(format,this);

    connect(audOut,SIGNAL(stateChanged(QAudio::State)),this, SLOT(finishedPlaying(QAudio::State)));
    audOut->start(buf);

}

void CaptureAudio::finishedPlaying(QAudio::State state) {

    if (state == QAudio::IdleState) {
        audOut->stop();
        buffer->close();
        delete audOut;
    }
}
