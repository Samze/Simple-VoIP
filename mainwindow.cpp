#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setup connections
    audio = new CaptureAudio();
    sender = new SoundSender();

    connect(ui->btnCapture,SIGNAL(clicked()),audio,SLOT(recordSound()));

    connect(ui->btnStopCapture,SIGNAL(clicked()),audio,SLOT(stopRecording()));

    connect(audio, SIGNAL(hasVoiceData(QBuffer*)),audio,SLOT(playSound(QBuffer*)));
    connect(audio, SIGNAL(hasVoiceData(QBuffer*)),sender,SLOT(sendData(QBuffer*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete audio;
}

void MainWindow::test() {


}
