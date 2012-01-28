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
	
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!callGreen.load(":/images/In_call.png")) {
        qWarning("Failed to load :/images/In_call.png");
    }

    if(!callRed.load(":/images/no_call.png")) {
        qWarning("Failed to load :/images/no_call.png");
    }

    controller = new StateController();

    //setup start connections

    //GUI interaction
    connect(ui->btnCapture,SIGNAL(clicked()),this,SLOT(callPressed()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),controller,SLOT(endCall()));

    connect(ui->actionCall,SIGNAL(toggled(bool)),this,SLOT(callPressed()));

    connect(ui->chkMuteMic,SIGNAL(clicked(bool)),controller,SIGNAL(muteMic(bool)));
    connect(ui->chkMuteSound,SIGNAL(clicked(bool)),controller,SIGNAL(muteSound(bool)));
    connect(ui->actionMute_Mic,SIGNAL(triggered(bool)),controller,SIGNAL(muteMic(bool)));
    connect(ui->actionMute_Sound,SIGNAL(triggered(bool)),controller,SIGNAL(muteSound(bool)));

    //Keep them in sync
    connect(ui->chkMuteMic,SIGNAL(clicked(bool)),ui->actionMute_Mic,SLOT(setChecked(bool)));
    connect(ui->chkMuteSound,SIGNAL(clicked(bool)),ui->actionMute_Sound,SLOT(setChecked(bool)));
    connect(ui->actionMute_Mic,SIGNAL(triggered(bool)),ui->chkMuteMic,SLOT(setChecked(bool)));
    connect(ui->actionMute_Sound,SIGNAL(triggered(bool)),ui->chkMuteSound,SLOT(setChecked(bool)));

    connect(this,SIGNAL(call(QString)),controller,SLOT(callPeer(QString)));
    connect(this,SIGNAL(inCallAccepted()),controller,SLOT(acceptCall()));
    connect(this,SIGNAL(inCallRejected()),controller,SLOT(rejectCall()));

    //feedback from controller.
    connect(controller,SIGNAL(updatePeerList(QList<Peer*>)),this,SLOT(updateGUIPeerList(QList<Peer*>)));
    connect(controller,SIGNAL(callerBusy()),this,SLOT(callerBusy()));
    connect(controller,SIGNAL(callError(QString)),this,SLOT(callError(QString)));
    connect(controller,SIGNAL(callIncoming(QString)),this,SLOT(incomingCall(QString)));

    connect(controller,SIGNAL(callerMicMuted(bool)),this,SLOT(callerMicMuted(bool)));
    connect(controller,SIGNAL(callerSoundMuted(bool)),this,SLOT(callerSoundMuted(bool)));

    connect(controller,SIGNAL(newState(StateController::VoIPState)),this,SLOT(callStateChanged(StateController::VoIPState)));


    connect(ui->actionExit, SIGNAL(triggered()),qApp,SLOT(closeAllWindows()));
    connect(qApp,SIGNAL(aboutToQuit()),controller,SLOT(endCall()));

    //set edit options
    connect(ui->actionAbout_QT, SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    this->setWindowTitle("VoIP Application");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete controller;
}

void MainWindow::updateGUIPeerList(QList<Peer*> peerList)
{
  //Populate GUI list with peers
   peerNameList = Peer::getPeersNameList(peerList);
   ui->lstWidget->clear();
   ui->lstWidget->addItems(peerNameList);

}

void MainWindow::callPressed() {

    if (!ui->lstWidget->selectedItems().isEmpty()) {
        QString name = ui->lstWidget->selectedItems().takeFirst()->text();

        if (!name.isNull()) {
            emit call(name);
        }
    }
    else {
        QMessageBox::warning(this, "Call Error","You must select someone to call.",QMessageBox::Close,QMessageBox::Close);
    }
}


void MainWindow::callError(const QString &error) {
    QMessageBox::warning(this, "Call Error",error,QMessageBox::Close,QMessageBox::Close);

}

void MainWindow::incomingCall(const QString &name) {
    QString msg("Incoming call from : " + name + ", Do you wish to accept the call?");

    switch(QMessageBox::question(this, "",msg,
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No))
    {
    case QMessageBox::Yes:
        emit inCallAccepted();
        break;

    case QMessageBox::No:
        emit inCallRejected();
        break;

    default:
        emit inCallRejected();
        break;

    }
}

void MainWindow::callStateChanged(StateController::VoIPState state) {

    switch(state) {
        case StateController::InCall:
            ui->lblCallName->setText(controller->getCallPeer()->getName());
            ui->lblCallIcon->setPixmap(callGreen);
            break;
        case StateController::Ready:
            ui->lblCallName->setText("No Call");
            ui->lblCallIcon->setPixmap(callRed);
            ui->lblCallerSound->setText("");
            break;
        case StateController::Calling:
            ui->lblCallName->setText("Calling...");
    }

}

void MainWindow::callerBusy() {

    QMessageBox::warning(this,"","The person you tried to call is busy",QMessageBox::Close);
}

void MainWindow::callerMicMuted(bool toggle) {
    QString msg;
    msg = toggle ? controller->getCallPeer()->getName() + " muted their Mic." : "";
    ui->lblCallerMic->setText(msg);
}

void MainWindow::callerSoundMuted(bool toggle) {
    QString msg;
    msg = toggle ? controller->getCallPeer()->getName() + " muted their sound." : "";
    ui->lblCallerSound->setText(msg);
}


void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this,QString("%1 - %2").arg(qApp->applicationName(),qApp->applicationVersion()), "VoIP by Sam Gunaratne of University of Plymouth.\nComplete source code can be found at http://github.com/Samze/ ");
}
