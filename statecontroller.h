#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include <QObject>
#include "voiceinput.h"
#include "voiceoutput.h"
#include "soundsender.h"
#include "soundreciever.h"
#include "receivethread.h"
#include "sendthread.h"
#include "commandclient.h"
#include "commandserver.h"
#include "networkdiscover.h"

class StateController : public QObject
{
    Q_OBJECT
public:
    explicit StateController(QObject *parent = 0);
    ~StateController();

    enum VoIPState {
       Ready,
       Busy
    };

signals:
    void updatePeerList(QList<Peer*>);
    void callError(QString);

    void callIncoming(QString);
    void callResponse(int);

    void callerBusy();


public slots:
    void callPeer(QString);
    void endCall();
    void muteSound();
    void muteMic();

    void receiveCall(QHostAddress);
    void acceptCall();
    void rejectCall();

private:
    ReceiveThread *recThread;
    SendThread *sendThread;

    CommandClient *client;
    CommandServer *server;
    NetworkDiscover *discover;
    VoIPState state;

    QHostAddress incomingCaller;


    QString getNameFromAddress(const QHostAddress &);

};

#endif // STATECONTROLLER_H
