#include <QtGui/QApplication>
#include "mainwindow.h"

/*! \mainpage University of Plymouth SOFT336 Coursework by Sam Gunaratne
 *
 * \section About
 *
 * VoIP allows you to have real time two-way voice communication over a local network. The software has no centralized server and is entirely peer to peer driven.
 *
 */

//\subsection Name Stuff here

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QIcon icon(":/images/headphones.png");
    a.setWindowIcon(icon);
    a.setApplicationName("VoIP ");
    a.setApplicationVersion("1.0");

    a.setOrganizationName("University of Plymouth");
    a.setOrganizationDomain("http://github.com/Samze/");

    MainWindow w;
    w.show();

    return a.exec();
}
