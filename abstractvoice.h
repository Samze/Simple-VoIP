#ifndef ABSTRACTVOICE_H
#define ABSTRACTVOICE_H

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QDebug>

/**
 * Abstracted common voice elements for recording and playback. This class contains common attributes and virtual functions
 * for both recording and playing audio. It holds information about the local device detected, a default format
 * and a buffer for which will be written or read.
 */
class AbstractVoice : public QObject
{
    Q_OBJECT

public:
    /**
      * Abstract constructor. Instansiates a format and device object.
      */
    explicit AbstractVoice(QObject *parent = 0);

    /**
      * Abstract destructor. Deletes the device and buffer objects. The buffer
      * object must be instansiated in a subclass.
      */
    ~AbstractVoice();

public slots:
    /**
      * A virtual function that starts the voice operation. This function must
      * be implemented.
      */
    virtual void start() = 0;

    /**
      * A virtual function that stops all voice operation and subsequently is ready to receive another
      * start call. This function must be implemented.
      */
    virtual void stop() = 0;

private slots:
    /**
      * A virtual function that handles a change of state in the audio device. This slot should be
      * implemented and also connected to any device's change of state.
      * @param state Enumerated audio state update.
      */
    virtual void audioState(QAudio::State state) = 0;

protected:
    QAudioFormat format; /**< Audio format to be used, default is set to audio/pcm, 8000 freq, 1 channel, 8 sample rate.*/
    QAudioDeviceInfo *devInfo; /**< Populated with the default audio device detected */
    QBuffer *buffer; /**< The buffer to be used, must be implemented by subclass. */
};

#endif // ABSTRACTVOICE_H
