#include "voiceres.h"

voiceRes::voiceRes(QObject *parent) : QObject(parent)
{

}

void voiceRes::give_voice_startgame()
{
    voiceRes voice;
    voice.voiceStartgame->play();
}

void voiceRes::give_voice_buygood()
{
    voiceRes voice;
    voice.voiceBuygame->play();
}
