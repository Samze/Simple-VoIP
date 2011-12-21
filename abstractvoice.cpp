#include "abstractvoice.h"

AbstractVoice::AbstractVoice(QObject *parent) : QObject(parent)
{
    //This is the default format which works locally with no special codec/advanced compression.
    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);

    format.setCodec("audio/pcm"); //according to the docs this is the codec supported by all platforms.
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    //By default we'll get the platforms default device, could improve this and let the user pick later.
    devInfo = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());

    //Check support for format
    if (!devInfo->isFormatSupported(format)) {
        qWarning()<<"default format not supported, trying to use nearest";
        format = devInfo->nearestFormat(format);
    }
}

AbstractVoice::~AbstractVoice () {
    delete devInfo;
    delete buffer;
}
