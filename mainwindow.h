/*	
    VoIPApp- A simple p2p voip application written in C++ using the QT Framework.
    Copyright (C) 2011  Sam Gunaratne University of Plymouth

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
	
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "statecontroller.h"
#include <QMessageBox>

namespace Ui {
    class MainWindow;
}

/**
  * Graphical interface class of type QMainWindow. This class instansiates and holds logic for
  * the ui. It communicates with the state controller but has been made as decoupled as possible.
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
      * Construction which sets up the UI class as well as all signals/slots and instansiates
      * the controller.
      *@param parent Parent for this object.
      */
    explicit MainWindow(QWidget *parent = 0);

    /**
      * Default constructor which deletes the ui and all allocated objects.
      */
    ~MainWindow();

signals:
    void call(QString); /**< Signal emiited when the user has selected to start a new call. Provides the name selected.*/
    void inCallAccepted(); /**< Informs anyone interested that the call was accepted.*/
    void inCallRejected(); /**< Informs anyone interested that the call was rejected. */

private slots:

    /**
     * Updates the graphical view of the peer list.
     *@param pList The list of peers.
     */
     void updateGUIPeerList(QList<Peer*> pList);

     /**
      * User initiated a call.
      */
     void callPressed();

     /**
      * An error has occured for the user to be informed about. This will be popped up in a dialog.
      *@param error The error message.
      */
     void callError(const QString & error);

     /**
      * The caller was busy, pop up dialog.
      */
     void callerBusy();

     /**
      *
      * A new call is incoming, ask user's permission.
      *@param caller The name of the caller.
      */
     void incomingCall(const QString & caller);

     /**
      * Updates the GUI to reflect the new state.
      *@param state The new state.
      */
     void callStateChanged(StateController::VoIPState state);

     /**
       * The caller has toggled their microphone. This slot will updated the GUI accordingly.
       */
     void callerMicMuted(bool);

     /**
       * The caller has toggled their sound. This slot will updated the GUI accordingly.
       */
     void callerSoundMuted(bool);

     /**
       * UI element to display information about the application.
       */
     void on_actionAbout_triggered();
private:
    Ui::MainWindow *ui; /**< UI class */
    StateController* controller; /**< Controller resposible for all interaction*/

    //Pixmaps are icons used to update the GUI if were in a call or not.
    QPixmap callGreen; /**< Green icon, showing in call */
    QPixmap callRed; /**< Red icon, showing out of call */

    QStringList peerNameList; /**< A List of peers names to be displayed in a widget */
};

#endif // MAINWINDOW_H
