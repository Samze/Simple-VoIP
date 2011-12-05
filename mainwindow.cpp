#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setup connections
    audio = new CaptureAudio();
   // sender = new SoundSender();
    output = new VoiceOutput();

    connect(ui->btnCapture,SIGNAL(clicked()),audio,SLOT(recordSound()));
    connect(ui->btnCapture,SIGNAL(clicked()),output,SLOT(playSound()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),audio,SLOT(stopRecording()));

  //  connect(audio, SIGNAL(hasVoiceData(QBuffer*)),sender,SLOT(sendData(QBuffer*)));

   //connect(audio, SIGNAL(hasVoiceData(QBuffer*)),output,SLOT(playSound(QBuffer*)));

   // connect (receiver, SIGNAL(dataReceived(QBuffer*)),output,SLOT(playSound(QBuffer*)));

    //start our threads
    //sender->start(); don't need this
    //receiver->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete audio;
    delete sender;
    delete output;
}

void MainWindow::test() {


}
