#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include <QObject>
#include <QApplication>
#include "voiceinput.h"
#include "voiceoutput.h"
#include "soundsender.h"
#include "soundreciever.h"
#include "receivethread.h"
#include "sendthread.h"
#include "commandclient.h"
#include "commandserver.h"
#include "networkdiscover.h"

/**
  * This controller object contains state information and acts as a go between the view and model. This class acts as a tyypical
  * controller object. It receives commands from user interaction and via the network and performs actions accordingly. It has a number of signals that UIs
  * should connect to in order to receive information. It controls the starting and stopping of threads that perform all the voice communication.
  */
class StateController : public QObject
{
    Q_OBJECT
public:

    /**
     * Possible VoIP states for the application to be in. A List of enumerated states that the
     *application can possibly be in. This is used to represent a finite-state machine.
     */
    enum VoIPState {
       Ready, /**< Ready state shows that the application can receive calls. */
       Calling, /**< The application is waiting on calling another user. */
       InCall /**< The application is in a call with another user. */
    };

    /**
      * Constructs all necessary objects and connects signals and slots.
      *@param parent Owner of this object
      */
    explicit StateController(QObject *parent = 0);

    /**
      * Default destructor.
      */
    ~StateController();

    /**
      * Gets the peer being communicated with.
      *@return Returns the peer being communicated with.
      */
    Peer* getCallPeer() { return commPeer;}

signals:
    void updatePeerList(QList<Peer*>); /**< Signal emitted when an updated peer list is available . */

    void callIncoming(QString); /**< Signal emitted when a call is being requested to initiate. Signal stores the callers username. */

    void callerBusy(); /**< Emitted when a call was initiated but the caller was busy.*/
    void newState(StateController::VoIPState); /**< The application has entered a new state. */

    void muteSound(bool); /**< User has initiated that their sound has been toggled, this signal will be emitted and a message sent to caller.*/
    void muteMic(bool); /**< User has initiated that their mic has been toggled, this signal will be emitted and a message sent to caller.*/

    void callerMicMuted(bool); /**< Emitted when a caller has toggled their mic state.*/
    void callerSoundMuted(bool); /**< Emitted when a caller has toggled their sound state*/

    void callError(QString); /**< Emitted when an error has occured.*/

public slots:
    /**
      * UIs should connect to this slot with the name of the person they wish to call, the controller will
      * initiate the call.
      * @param name The username of the peer to call.
      */
    void callPeer(QString name);

    /**
      * UIs should conect to this slot when they wish to accept a call. This will inform the caller and begin the
      * process of sending/receiving.
      */
    void acceptCall();

    /**
      * UIs should conect to this slot when they wish to reject a call. This will inform the caller.
      */
    void rejectCall();

    /**
      * UIs should conect to this slot when they wish to end a call. This will end call activies and inform the caller.
      */
    void endCall();

private slots:
    void outCallAccepted(); /**< Private slot notified when we initated a call and it was accepted. This emits the new state. */
    void outCallRejected(); /**< Private slot notified when we initated a call and it was rejected. This emits the new state and callerBusy*/
    void sendMuteSound(bool); /**< Private slot that informs callers that we toggled our sound state. */
    void sendMuteMic(bool); /**< Private slot that informs callers that we toggled our mic state. */
    void receiveCall(const QHostAddress &); /**< Private slot when we get a new call, this emits callIncoming.*/

private:
    VoIPState state;  /**< Holds the state of our application*/
    Peer* commPeer;  /**< The current peer in communication with. */

    ReceiveThread *recThread; /**< Our receieve thread object, used for receiving/playing data*/
    SendThread *sendThread;  /**< Our send thread object, used for capturing/sending*/

    CommandClient *client;  /**< Commandclient, for when we initiate calls*/
    CommandServer *server;  /**< Commandserver, for when we receive call requests*/

    NetworkDiscover *discover;  /**< Discovers peers*/
};

#endif // STATECONTROLLER_H
