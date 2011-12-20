#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "statecontroller.h"
#include <QMessageBox>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void call(QString);

    void inCallAccepted();
    void inCallRejected();

public slots:
     void updateGUIPeerList(QList<Peer*>);
     void callPressed();
     void callError(const QString &);
     void callerBusy();
     void incomingCall(const QString &);
     void setCallStatus(StateController::VoIPState);

private slots:
     void callerMicMuted(bool);
     void callerSoundMuted(bool);

private:
    Ui::MainWindow *ui;
    StateController* controller;

    QStringList list;
};

#endif // MAINWINDOW_H
