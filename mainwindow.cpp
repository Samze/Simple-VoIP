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

    recThread = new ReceiveThread();


    connect(ui->btnCapture,SIGNAL(clicked()),audio,SLOT(recordSound()));
    //connect(ui->btnCapture,SIGNAL(clicked()),output,SLOT(playSound()));
    connect(ui->btnCapture,SIGNAL(clicked()),recThread,SLOT(listen()));
    connect(ui->btnStopCapture,SIGNAL(clicked()),audio,SLOT(stopRecording()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete audio;
    //delete output;
}

void MainWindow::test() {


}
