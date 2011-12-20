#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "peer.h"


class CommandClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CommandClient(QObject *parent = 0);

    enum CallCommand {
       Call,
       CallAccepted,
       Busy,
       EndCall,
       MuteMic,
       MuteSound
    };

signals:
    void connectionEstablished();
    void callerAccepted();
    void callerBusy();
    void callEnded();
    void commandError();

public slots:
    void connectToPeer(Peer*);
    void hangUp();
    void callPeer();
    void serverResponse();

private:
    static quint16 generateUDPPort();
    static const quint16 UDPMAX = 45999;
    static const quint16 UDPMIN = 44000;

};

#endif // COMMANDCLIENT_H
