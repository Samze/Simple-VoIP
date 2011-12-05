#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "captureaudio.h"
#include "soundsender.h"
#include "soundreciever.h"
#include "voiceoutput.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void test();

private:
    Ui::MainWindow *ui;
    CaptureAudio *audio;
    SoundSender *sender;
    SoundReciever *receiver;
    VoiceOutput *output;
};

#endif // MAINWINDOW_H
