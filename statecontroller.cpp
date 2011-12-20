#include "statecontroller.h"

StateController::StateController(QObject *parent) :
    QObject(parent)
{

    //setup tcp listening for commands....

    client = new CommandClient(this);
    server = new CommandServer(this);

    discover = new NetworkDiscover(this);
    discover->broadCastTimer.start();
    discover->peerCheck.start();

    //setup broadcast p2p nature...

    //setup two threads for sending and receiving audio data
    recThread = new ReceiveThread(this);
    sendThread = new SendThread(this);

    connect(server,SIGNAL(callInitiated(QHostAddress)),this,SLOT(receiveCall(QHostAddress)));

    //setup stop connections
    connect(server,SIGNAL(callEnded()),this,SLOT(endCall()));

    connect(discover,SIGNAL(peersChanged(QList<Peer*>)),this,SIGNAL(updatePeerList(QList<Peer*>)));
    connect(client,SIGNAL(callerBusy()),this,SIGNAL(callerBusy()));


    state = StateController::Ready;
}

StateController::~StateController() {

    delete recThread;
    delete sendThread;
    delete client;
    delete server;
}

void StateController::callPeer(QString name) {

    if(state == StateController::Ready) {
        //Lets get this rolling!
        Peer* peer = discover->peerList.value(name);
        sendThread->recordSound(*peer->getAddress());
    }
    else {
        emit callError("You are already in a call. You must terminate before calling someone else");
    }
}

void StateController::endCall() {
    //Ended calls are processed at the controller level instead of directly at the
    //model (ie. listen/send threads) just incase we want to update the view with anthing later...probably won't use this

    recThread->quit();
    sendThread->quit();
}

void StateController::muteSound() {
    //TODO
}

void StateController::muteMic() {
    //TODO
}

void StateController::receiveCall(QHostAddress address) {
    //Again this could have been achieved by connecting the direct received calls to this acton, but we want to check
    //the state and perhaps perform gui operations. Controller assumes responsability.

    if (state == StateController::Ready) {

        QString name = getNameFromAddress(address);
        if (!name.isNull()) {
            incomingCaller = address; //store the address of the incoming caller for when the view responses.
            emit callIncoming(name);
        }
    }
    else {
        //server response here..
    }
}

QString StateController::getNameFromAddress(const QHostAddress &address) {

    foreach(Peer* p, discover->peerList.values()) {
        if (p->getAddress() == address) {
            return p->getName();
        }
    }
    return null;
}

void StateController::acceptCall() {
    sendThread->recordSound(incomingCaller);
    recThread->listen();

    //server responses here?

    incomingCaller = null;
}


void StateController::rejectCall() {

    //server reponses here?
    server->busy(incomingCaller);

    incomingCaller = null;
}
