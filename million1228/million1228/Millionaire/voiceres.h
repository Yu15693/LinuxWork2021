#ifndef VOICERES_H
#define VOICERES_H

#include <QObject>
#include <QSound>
class voiceRes : public QObject
{
    Q_OBJECT
public:
    explicit voiceRes(QObject *parent = 0);

    QSound *voiceStartgame = new QSound(":/img/voiceRes/BackButtonSound.wav");//开始按键的音效
    QSound *voiceBuygame = new QSound(":/img/voiceRes/buyVoice.wav");//购买按键的音效

signals:

public slots:
    void give_voice_startgame();
    void give_voice_buygood();
};

#endif // VOICERES_H
