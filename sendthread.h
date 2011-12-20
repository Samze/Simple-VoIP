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
    QHostAddress m_address;

signals:
    void muteMic(bool);

protected:
    void run();

public slots:
    void recordSound(const QHostAddress &);

};

#endif // SENDTHREAD_H
