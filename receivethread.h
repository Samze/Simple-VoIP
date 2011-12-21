#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include "voiceoutput.h"

/**
  * A Thread class that controls listening for incoming voice data. This class listens for incoming UDP
  * and then plays it using the Computers default audio out device. This class inherits from QThread.
  */
class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    /**
      * Default constructor.
      *@param parent The owner of this object.
      */
    explicit ReceiveThread(QObject *parent = 0);

protected:
    /**
      *Reimplmented function from QThread that beings receiving and playback.
      */
    void run();

signals:
    void muteSound(bool); /**< Signal that should emitted when a request to mute is processed. This class will then mute the sound. */

public slots:
    /**
      * Starts the new thread.
      */
    void listen();

};

#endif // RECEIVETHREAD_H
