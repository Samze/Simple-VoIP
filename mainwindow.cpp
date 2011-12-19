#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    //setup start connections
    connect(ui->btnCapture,SIGNAL(clicked()),client,SLOT(connectToPeer()));

    connect(ui->btnCapture,SIGNAL(clicked()),this,SLOT(callPeer()));
    connect(ui->btnCapture,SIGNAL(clicked()),recThread,SLOT(listen()));

    connect(ui->btnStopCapture,SIGNAL(clicked()),client,SLOT(hangUp()));

    connect(server,SIGNAL(callInitiated(QHostAddress)),sendThread,SLOT(recordSound(QHostAddress)));
    connect(server,SIGNAL(callInitiated(QHostAddress)),recThread,SLOT(listen()));

    //setup stop connections
    connect(server,SIGNAL(callEnded()),sendThread,SLOT(quit()));
    connect(server,SIGNAL(callEnded()),recThread,SLOT(quit()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),sendThread,SLOT(quit()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),recThread,SLOT(quit()));
    connect(discover,SIGNAL(peersChanged(QList<Peer*>)),this,SLOT(output(QList<Peer*>)));
}


MainWindow::~MainWindow()
{
    delete ui;
    delete recThread;
    delete sendThread;
}

void MainWindow::output(QList<Peer*> peerList) {

   list = Peer::getPeersNameList(peerList);
   ui->lstWidget->clear();
   ui->lstWidget->addItems(list);

}

void MainWindow::callPeer() {

    QString name = ui->lstWidget->selectedItems().takeFirst()->text();

    Peer* peer = discover->peerList.value(name);
    sendThread->recordSound(*peer->getAddress());
}
