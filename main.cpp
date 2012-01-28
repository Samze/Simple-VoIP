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
