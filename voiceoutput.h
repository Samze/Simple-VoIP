#ifndef VOICEOUTPUT_H
#define VOICEOUTPUT_H

#include <QIODevice>
#include <QThread>


class VoiceOutput : public QIODevice
{
    Q_OBJECT
public:
    explicit VoiceOutput(QObject *parent = 0);

signals:

public slots:

};

#endif // VOICEOUTPUT_H
