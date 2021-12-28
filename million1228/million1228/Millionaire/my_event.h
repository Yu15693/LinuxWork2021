#ifndef MY_EVENT_H
#define MY_EVENT_H
#include <QVector>

class game_status;

class my_event
{
public:
    game_status *s;
    //定义函数指针数组
    typedef QString(my_event::*fun)(void);
    QVector<fun> events;

    my_event(game_status *);
    //触发事件
    QString trigger_event();
    //各类事件
    QString custom_event_0();
    QString custom_event_1();
    QString custom_event_2();
    QString custom_event_3();
    QString custom_event_4();
    QString custom_event_5();

};

#endif // MY_EVENT_H
