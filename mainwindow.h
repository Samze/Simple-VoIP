#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "voiceinput.h"
#include "voiceoutput.h"
#include "soundsender.h"
#include "soundreciever.h"
#include "receivethread.h"
#include "sendthread.h"
#include "commandclient.h"
#include "commandserver.h"
#include "networkdiscover.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ReceiveThread *recThread;
    SendThread *sendThread;
};

#endif // MAINWINDOW_H
