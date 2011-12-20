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
    connect(client,SIGNAL(callEnded()),this,SLOT(endCall()));

    //Initiated call busy.
    connect(client,SIGNAL(callerAccepted()),this,SLOT(outCallAccepted()));
    connect(client,SIGNAL(callerBusy()),this,SLOT(callerWasBusy()));

    connect(client,SIGNAL(callerMicMuted(bool)),this,SIGNAL(callerMicMuted(bool)));
    connect(server,SIGNAL(callerMicMuted(bool)),this,SIGNAL(callerMicMuted(bool)));

    connect(client,SIGNAL(callerSoundMuted(bool)),this,SIGNAL(callerSoundMuted(bool)));
    connect(server,SIGNAL(callerSoundMuted(bool)),this,SIGNAL(callerSoundMuted(bool)));

    connect(this,SIGNAL(muteSound(bool)),recThread,SIGNAL(muteSound(bool)));
    connect(this,SIGNAL(muteMic(bool)),sendThread,SIGNAL(muteMic(bool)));

    connect(this,SIGNAL(muteSound(bool)),this,SLOT(sendMuteSound(bool)));
    connect(this,SIGNAL(muteMic(bool)),this,SLOT(sendMuteMic(bool)));

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

        CommandClient* clientSender = dynamic_cast<CommandClient*>(sender());
        CommandServer* serverSender = dynamic_cast<CommandServer*>(sender());

        //Reqiures compiler rtti support!
        if (clientSender == 0 && serverSender ==0) {

            if (client->state() == QTcpSocket::ConnectedState)
                client->hangUp();
            else
                server->sendCommand(*incomingCaller,CommandClient::EndCall);
        }

        recThread->quit();
        sendThread->quit();
        state = StateController::Ready;
        emit newState(state);
        //TODO deal with incomingCaller here too, delete?
    }
}


void StateController::receiveCall(const QHostAddress &address) {
    //This could have been achieved by connecting the direct received calls to this acton, but we want to check
    //the state and perhaps perform gui operations. Controller assumes responsability.

    if (state == StateController::Ready) {

        qDebug() << &address;
        QString name = getNameFromAddress(address);
        if (!name.isNull()) {
            incomingCaller = new QHostAddress(address); //store the address of the incoming caller for when the view responses.
            qDebug() << incomingCaller;

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
    server->sendCommand(*incomingCaller,CommandClient::CallAccepted);

    //Begin send/listen.
    sendThread->recordSound(*incomingCaller);
    recThread->listen();

    state = StateController::InCall;

    emit newState(state);
}


void StateController::rejectCall() {

    //server reponses here?
    server->sendCommand(*incomingCaller,CommandClient::Busy);

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


void StateController::sendMuteSound(bool toggle) {

    CommandClient::CallCommand cmd;

    cmd = toggle ? CommandClient::disableSound : CommandClient::enableSound;

    if (client->state() == QTcpSocket::ConnectedState)
        client->sendCommand(cmd);
    else
        server->sendCommand(*incomingCaller,cmd);
}

void StateController::sendMuteMic(bool toggle) {
    CommandClient::CallCommand cmd;

    cmd = toggle ? CommandClient::disableMic : CommandClient::enableMic;

    if (client->state() == QTcpSocket::ConnectedState)
        client->sendCommand(cmd);
    else
        server->sendCommand(*incomingCaller,cmd);
}
