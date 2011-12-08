#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include "voiceoutput.h"

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(QObject *parent = 0);

protected:
    void run();

public slots:
    void listen();

private:
    //VoiceOutput* output;

};

#endif // RECEIVETHREAD_H
