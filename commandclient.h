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
       disableMic,
       disableSound,
       enableMic,
       enableSound
    };

signals:
    void connectionEstablished();
    void callerAccepted();
    void callerBusy();
    void callEnded();
    void commandError();
    void callerMicMuted(bool);
    void callerSoundMuted(bool);

public slots:
    void sendCommand(CallCommand cmd);
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
