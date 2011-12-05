#include "captureaudio.h"
#include "soundsender.h"

CaptureAudio::CaptureAudio()
{
    format.setFrequency(4000);
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
}

CaptureAudio::~CaptureAudio() {
    delete devInfo;
}

void CaptureAudio::recordSound() {

    buffer = new QBuffer(this);
    buffer->open(QIODevice::ReadWrite);
    //setup audio
    audioDev = new QAudioInput(format, this);

    //connect(audioDev,SIGNAL(stateChanged(QAudio::State)),this,SLOT(audioActive(QAudio::State)));

    SoundSender *s = new SoundSender();
    s->open(QIODevice::ReadWrite);

    audioDev->start(s);

}

//void CaptureAudio::audioActive(QAudio::State state) {

//    audioDev->bytesReady();
//    if (state == QAudio::ActiveState) {
//        qDebug("we got data!");
//        buffer->readAll();
//       if( buffer->bytesAvailable()) {
//           qDebug("we got data, sadly!");

//            }
//    }
//}

void CaptureAudio::stopRecording() {
    audioDev->stop();
    buffer->close();

//    qDebug("Bytes available = %ld",buffer->bytesAvailable());


//    QByteArray array = buffer->data();

//    for (int i=0; i < array.size(); ++i) {
//       int chars = static_cast<int>(array.at(i));
//        qDebug("%d",chars);
//    }

    //send data, play sound
    //emit hasVoiceData(buffer);
    //delete audioDev;
}


