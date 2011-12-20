#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include "voiceoutput.h"

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(QObject *parent = 0);
    ~ReceiveThread();
protected:
    void run();

signals:
    void muteSound(bool);

public slots:
    void listen();

};

#endif // RECEIVETHREAD_H
