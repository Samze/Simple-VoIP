#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    controller = new StateController();

    //setup start connections
    connect(ui->btnCapture,SIGNAL(clicked()),this,SLOT( ));
    connect(this,SIGNAL(call(QString)),controller,SLOT(callPeer(QString)));

    connect(ui->btnCapture,SIGNAL(clicked()),client,SLOT(connectToPeer()));


    connect(controller,SIGNAL(updatePeerList(QList<Peer*>)),this,SLOT(updateGUIPeerList(QList<Peer*>));

    connect(ui->btnCapture,SIGNAL(clicked()),this,SLOT(callPeer()));
    connect(ui->btnCapture,SIGNAL(clicked()),recThread,SLOT(listen()));

    connect(ui->btnStopCapture,SIGNAL(clicked()),client,SLOT(hangUp()));

    connect(ui->btnStopCapture,SIGNAL(clicked()),sendThread,SLOT(quit()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),recThread,SLOT(quit()));

    connect(this,SIGNAL(inCallAccepted()),controller,SLOT(acceptCall()));
    connect(this,SIGNAL(inCallRejected()),controller,SLOT(rejectCall()));

    connect(controller,SIGNAL(callerBusy()),this,SLOT(callerBusy()));
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

    QString name = ui->lstWidget->selectedItems().takeFirst()->text();

    if (!name.isNull()) {
        emit call(name);
    }
}


void MainWindow::callError(QString error) {
    QMessageBox::warning(this,&error,QMessageBox::Close);

}

void MainWindow::incomingCall(QString name) {
    QString msg("Incoming call from : " + name + " Do you wish to accept the call?");

    switch(QMessageBox::question(this, &msg,
    QMessageBox::Yes | QMessageBox::No,
    QMessageBox::No))
    {
        case QMessageBox::Yes:
            emit inCallAccepted();
        break;

        case QMessageBox::No:
            emit inCallRejected();
        break;

    }
}


void MainWindow::callerBusy() {

    QMessageBox::warning(this,"The person you tried to call is busy",QMessageBox::Close);
}

