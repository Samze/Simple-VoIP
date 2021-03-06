/*	
    VoIPApp- A simple p2p voip application written in C++ using the QT Framework.
    Copyright (C) 2011  Sam Gunaratne University of Plymouth

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
	
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
    connect(client,SIGNAL(callerBusy()),this,SLOT(outCallRejected()));

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
    discover->startTimers();

    //set initial state
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

        commPeer = discover->peerList.value(name);
        client->connectToPeer(commPeer);

        state = StateController::Calling;
        emit newState(state);
    }
    else {
        emit callError("You are already in a call. You must terminate before calling someone else");
    }
}

void StateController::endCall() {

    //Ended calls are processed at the controller level instead of directly at the
    //model (ie. listen/send threads) just incase we want to update the view with anthing later...probably won't use this

    if (state == InCall) {

        //Find out which signal emitted. If the signal did not come from the network we need to tell
        //our caller to end their call.
        CommandClient* clientSender = dynamic_cast<CommandClient*>(sender());
        CommandServer* serverSender = dynamic_cast<CommandServer*>(sender());

        QApplication* app = dynamic_cast<QApplication*>(sender());

        //Reqiures compiler rtti support!
        if (clientSender == 0 && serverSender ==0) {

            if (client->state() == QTcpSocket::ConnectedState)
                client->hangUp();
            else
                server->sendCommand(*commPeer->getAddress(),CommandClient::EndCall);
        }

        //if qApp emitted us we do not want to end threads. This causes segmentation fault.
        if(app == 0) {
            if (recThread->isRunning()) {
                recThread->quit();
            }
            if (sendThread->isRunning()) {
                sendThread->quit();
            }
        }

        state = StateController::Ready;
        emit newState(state);
        //TODO deal with incomingCaller here too, delete?
    }
}


void StateController::receiveCall(const QHostAddress &address) {
    //This could have been achieved by connecting the direct received calls to this acton, but we want to check
    //the state and perhaps perform gui operations. Controller assumes responsability.

    if (state == StateController::Ready || state == StateController::Calling) {

        commPeer = discover->getPeerFromAddress(address);

        if (!commPeer->getName().isNull()) {
            emit callIncoming(commPeer->getName());
        }
    }
    else {
        rejectCall();
    }
}

void StateController::acceptCall() {

    //Inform caller that we accept the call.
    server->sendCommand(*commPeer->getAddress(),CommandClient::CallAccepted);

    //Begin send/listen.
    sendThread->recordSound(*commPeer->getAddress());
    recThread->listen();

    state = StateController::InCall;

    emit newState(state);
}


void StateController::rejectCall() {
    //server reponses here?
    server->sendCommand(*commPeer->getAddress(),CommandClient::Busy);
}

void StateController::outCallAccepted() {

    if (state == StateController::Calling) {
        sendThread->recordSound(*commPeer->getAddress());
        recThread->listen();

        state = StateController::InCall;

        emit newState(state);
    }
}

void StateController::outCallRejected() {

    state = StateController::Ready;

    emit newState(state);
    emit callerBusy();
}

void StateController::sendMuteSound(bool toggle) {

    if (state == StateController::InCall) {
        CommandClient::CallCommand cmd;

        cmd = toggle ? CommandClient::disableSound : CommandClient::enableSound;

        //At the moment we check to see if we are client or server and respond
        //down the correct avenue. Ideally we should only have to respond down one channel. Makes for bloated code.
        if (client->state() == QTcpSocket::ConnectedState)
            client->sendCommand(cmd);
        else
            server->sendCommand(*commPeer->getAddress(),cmd);
     }
}

void StateController::sendMuteMic(bool toggle) {

    if (state == StateController::InCall) {
        CommandClient::CallCommand cmd;

        cmd = toggle ? CommandClient::disableMic : CommandClient::enableMic;

        if (client->state() == QTcpSocket::ConnectedState)
            client->sendCommand(cmd);
        else
            server->sendCommand(*commPeer->getAddress(),cmd);
    }
}


