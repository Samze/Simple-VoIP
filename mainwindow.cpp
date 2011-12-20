#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    controller = new StateController();

    //setup start connections

    //GUI interaction
    connect(ui->btnCapture,SIGNAL(clicked()),this,SLOT(callPressed()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),controller,SLOT(endCall()));

    connect(this,SIGNAL(call(QString)),controller,SLOT(callPeer(QString)));
    connect(this,SIGNAL(inCallAccepted()),controller,SLOT(acceptCall()));
    connect(this,SIGNAL(inCallRejected()),controller,SLOT(rejectCall()));

    //feedback from controller.
    connect(controller,SIGNAL(updatePeerList(QList<Peer*>)),this,SLOT(updateGUIPeerList(QList<Peer*>)));
    connect(controller,SIGNAL(callerBusy()),this,SLOT(callerBusy()));
    connect(controller,SIGNAL(callError(QString)),this,SLOT(callError(QString)));
    connect(controller,SIGNAL(callIncoming(QString)),this,SLOT(incomingCall(QString)));

    //the gui should do something with controller emitted signal
    //callAccepted()
    connect(controller,SIGNAL(newState(StateController::VoIPState)),this,SLOT(setCallStatus(StateController::VoIPState)));


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

   list = Peer::getPeersNameList(peerList);
   ui->lstWidget->clear();
   ui->lstWidget->addItems(list);

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

void MainWindow::setCallStatus(StateController::VoIPState state) {

    switch(state) {
        case StateController::InCall:
            ui->lblStatus->setText("In call");
            break;
        case StateController::Ready:
            ui->lblStatus->setText("Ready");
            break;
    }

}

