#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "peer.h"

/**
 * A subclass of QTcpSocket, class that is responsible for sending/receiving control signals between callers over the network.
 * This class is used to initiate calls to other users, it does this using the reliable protocol, TCP.
 * A series of commands have been enumerated for use which the corresponding server can understand. This class
 * can be reused and does not have to be re-instansiated for each new request or new peer.
 * This class inherits from QTcpSocket.
 */
class CommandClient : public QTcpSocket
{
    Q_OBJECT
public:
    /**
     * Call command enumerated for sending across the network.
     * A list of enumerated commands that can be sent from the client to the server.
     */
    enum CallCommand {
       Call, /**< initiate new call. */
       CallAccepted, /**< call has been accepted */
       Busy, /**< currently busy */
       EndCall, /**< terminating call */
       disableMic,/**< mic disabled */
       disableSound,/**< sound disabled */
       enableMic, /**< mic enabled */
       enableSound /**< sound enabled */
    };

    /**
     * Command Client constructor. Sets up a new command client class.
     * @param parent Parent for this object.
     */
    explicit CommandClient(QObject *parent = 0);


signals:
    void callerAccepted(); /**< Emitted when a call has been accepted*/
    void callerBusy(); /**< Emitted when a call has been rejected*/
    void callEnded(); /**< Emitted when a call has been terminated*/
    void commandError(); /**< Emitted when a command has not been understood.*/
    void callerMicMuted(bool); /**< Emitted when the caller has changed the state of their microphone. */
    void callerSoundMuted(bool); /**< Emitted when the caller has changed the state of their sound playback. */

public slots:
    /**
     * Allows client to send commands to the server. A connection must have
     * already be established.
     * @param cmd The command to send to the server.
     */
    void sendCommand(CallCommand cmd);

    /**
     * Initiates a connection to a peer.
     * @param p The peer that should be connected to.
     */
    void connectToPeer(Peer* p);

    /**
     * Hangs up on the current call. Note this performs the same operation as
     * send command but also also disconnects from the host.
     * @see disconnectFromHost()
     */
    void hangUp();

    /**
     * Initiates a call to a peer.
     * @see disconnectFromHost()
     */
    void callPeer();

private slots:
    /**
     * Handles potential repsonses from the server.
     */
    void serverResponse();

private:
    static quint16 generateUDPPort(); /**< NOT USED. Will generate a random port. */
    static const quint16 UDPMAX = 45999; /**< NOT USED. Max rnd udp port*/
    static const quint16 UDPMIN = 44000; /**< NOT USED. Min rnd udp port*/
};

#endif // COMMANDCLIENT_H
