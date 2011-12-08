#include "captureaudio.h"
#include <Qfile>

CaptureAudio::CaptureAudio()
{

    s = new SoundSender();
    format.setFrequency(44100);
    format.setChannels(2);
    format.setSampleSize(16);
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

    //setup audio
    audioDev = new QAudioInput(format, this);

   // audioDev->setBufferSize(4096);
   // connect(audioDev,SIGNAL(stateChanged(QAudio::State)),this,SLOT(audioActive(QAudio::State)));

    s->open(QIODevice::WriteOnly);

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
    delete audioDev;

}


