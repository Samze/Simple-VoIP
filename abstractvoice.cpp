#include "abstractvoice.h"

AbstractVoice::AbstractVoice(QObject *parent) : QObject(parent)
{
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
}
