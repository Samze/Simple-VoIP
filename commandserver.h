#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include <QObject>

class CommandServer : public QObject
{
    Q_OBJECT
public:
    explicit CommandServer(QObject *parent = 0);

signals:

public slots:

};

#endif // COMMANDSERVER_H
