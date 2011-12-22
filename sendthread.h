#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include "voiceinput.h"


/**
  * A Thread class that controls recording audio and sending data communication. This class records audio from the
  * default recording device and the sends them via UDP to the destination specified. This class inherits from QThread.
  */
class SendThread : public QThread
{
    Q_OBJECT
public:

    /**
      * Default constructor.
      *@param parent The owner of this object.
      */
    explicit SendThread(QObject *parent = 0);

signals:
    void muteMic(bool); /**< Signal that should emitted when a request to mute the mic is processed. This class will then mute the microphone. */

    /**
      *Reimplmented function from QThread that beings receiving and playback.
      */
protected:
    void run();

    /**
      * Starts the new thread, sending data to the given address.
      *@param addr The address to send the recorded audio.
      */
public slots:
    void recordSound(const QHostAddress & addr);

private:
    QHostAddress address; /**< locally stored address for thread access. */
};

#endif // SENDTHREAD_H
