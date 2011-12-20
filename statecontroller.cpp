#include "statecontroller.h"

StateController::StateController(QObject *parent) :
    QObject(parent)
{

    //setup tcp listening for commands....

    client = new CommandClient(this);
    server = new CommandServer(this);

    discover = new NetworkDiscover(this);

    //setup two threads for sending and receiving audio data
    recThread = new ReceiveThread(this);
    sendThread = new SendThread(this);

    //Incoming call
    connect(server,SIGNAL(callInitiated(QHostAddress)),this,SLOT(receiveCall(QHostAddress)));

    //Call terminated
    connect(server,SIGNAL(callEnded()),this,SLOT(endCall()));

    //Initiated call busy.
    connect(client,SIGNAL(callerAccepted()),this,SLOT(outCallAccepted()));
    connect(client,SIGNAL(callerBusy()),this,SLOT(callerWasBusy()));


    //Rebroadcast for the view.
    connect(discover,SIGNAL(peersChanged(QList<Peer*>)),this,SIGNAL(updatePeerList(QList<Peer*>)));

    //Start broadcast/listen for peer-to-peer nature
    discover->broadCastTimer.start();
    discover->peerCheck.start();

    state = StateController::Ready;
}

StateController::~StateController() {

    delete recThread;
    delete sendThread;
    delete client;
    delete server;
    delete discover;
}

void StateController::callPeer(QString name) {

    if(state == StateController::Ready) {
        //Lets get this rolling! initiate call.

        outgoingCall = discover->peerList.value(name)->getAddress();
        client->connectToPeer(discover->peerList.value(name));
    }
    else {
        emit callError("You are already in a call. You must terminate before calling someone else");
    }
}

void StateController::endCall() {
    //Ended calls are processed at the controller level instead of directly at the
    //model (ie. listen/send threads) just incase we want to update the view with anthing later...probably won't use this

    if (state == InCall) {
        client->hangUp();
        recThread->quit();
        sendThread->quit();

        state = StateController::Ready;
        emit newState(state);
    }
}

void StateController::muteSound() {
    //TODO
}

void StateController::muteMic() {
    //TODO
}

void StateController::receiveCall(const QHostAddress &address) {
    //This could have been achieved by connecting the direct received calls to this acton, but we want to check
    //the state and perhaps perform gui operations. Controller assumes responsability.

    if (state == StateController::Ready) {

        QString name = getNameFromAddress(address);
        if (!name.isNull()) {
            incomingCaller = new QHostAddress(address); //store the address of the incoming caller for when the view responses.
            emit callIncoming(name);
        }
    }
    else {
        rejectCall();
    }
}

QString StateController::getNameFromAddress(const QHostAddress &address) {

    foreach(Peer* p, discover->peerList.values()) {
        if (*p->getAddress() == address) {
            return p->getName();
        }
    }
    return NULL;
}

void StateController::acceptCall() {

    //Inform caller that we accept the call.
    server->sendCommand(*incomingCaller,CommandServer::Accepted);

    //Begin send/listen.
    sendThread->recordSound(*incomingCaller);
    recThread->listen();

    state = StateController::InCall;

    emit newState(state);
    delete incomingCaller;
}


void StateController::rejectCall() {

    //server reponses here?
    server->sendCommand(*incomingCaller,CommandServer::Busy);

    delete incomingCaller;
}

void StateController::outCallAccepted() {
    sendThread->recordSound(*outgoingCall);
    recThread->listen();

    state = StateController::InCall;

    emit newState(state);
}

void StateController::callerWasBusy() {
    emit callerBusy();
}
