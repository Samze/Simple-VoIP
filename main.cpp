#include <QtGui/QApplication>
#include "mainwindow.h"

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
