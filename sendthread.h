#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include "voiceinput.h"

class SendThread : public QThread
{
    Q_OBJECT
public:
    explicit SendThread(QObject *parent = 0);
    ~SendThread();

    QHostAddress* m_address;
protected:
    void run();

public slots:
    void recordSound(QHostAddress*);

};

#endif // SENDTHREAD_H
