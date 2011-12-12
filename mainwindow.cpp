#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setup tcp listening for commands....

    CommandClient *client = new CommandClient(this);
    CommandServer *server = new CommandServer(this);

    NetworkDiscover *discover = new NetworkDiscover(this);
    discover->broadCastTimer.start();
    discover->peerCheck.start();

    //setup broadcast p2p nature...

    //setup two threads for sending and receiving audio data
    recThread = new ReceiveThread(this);
    sendThread = new SendThread(this);

    //setup start connections
    connect(ui->btnCapture,SIGNAL(clicked()),sendThread,SLOT(recordSound()));
    connect(ui->btnCapture,SIGNAL(clicked()),recThread,SLOT(listen()));

    connect(ui->btnCapture,SIGNAL(clicked()),client,SLOT(callUser()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),client,SLOT(hangUp()));

    connect(server,SIGNAL(callInitiated()),sendThread,SLOT(recordSound()));
    connect(server,SIGNAL(callInitiated()),recThread,SLOT(listen()));

    //setup stop connections
    connect(server,SIGNAL(callEnded()),sendThread,SLOT(quit()));
    connect(server,SIGNAL(callEnded()),recThread,SLOT(quit()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),sendThread,SLOT(quit()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),recThread,SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete recThread;
    delete sendThread;
}
