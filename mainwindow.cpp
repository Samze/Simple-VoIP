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

    connect(ui->chkMuteMic,SIGNAL(clicked(bool)),controller,SIGNAL(muteMic(bool)));
    connect(ui->chkMuteSound,SIGNAL(clicked(bool)),controller,SIGNAL(muteSound(bool)));

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

    //the gui should do something with controller emitted signal
    //callAccepted()
    connect(controller,SIGNAL(newState(StateController::VoIPState)),this,SLOT(callStateChanged(StateController::VoIPState)));

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

void MainWindow::updateGUIPeerList(QList<Peer*> peerList) {

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
    QString msg("Incoming call from : " + name + " Do you wish to accept the call?");

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


void MainWindow::callerBusy() {

    QMessageBox::warning(this,"","The person you tried to call is busy",QMessageBox::Close);
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

void MainWindow::callerMicMuted(bool toggle) {
    qDebug() << "CallerMicMuted " << toggle;

    QString msg;
    msg = toggle ? controller->getCallPeer()->getName() + " muted their Mic." : "";
    ui->lblCallerMic->setText(msg);
}

void MainWindow::callerSoundMuted(bool toggle) {
    qDebug() << "CallerSoundMuted " << toggle;

    QString msg;
    msg = toggle ? controller->getCallPeer()->getName() + " muted their sound." : "";
    ui->lblCallerSound->setText(msg);
}
