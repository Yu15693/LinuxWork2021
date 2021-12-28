#include "my_event.h"
#include "game_status.h"
#include <QDebug>

my_event::my_event(game_status *_s)
{
    s = _s;
    //压入所有函数
    events.push_back(&custom_event_0);
    events.push_back(&custom_event_1);
    events.push_back(&custom_event_2);
    events.push_back(&custom_event_3);
    events.push_back(&custom_event_4);
    events.push_back(&custom_event_5);
}

QString my_event::trigger_event()
{
    int size = events.size();
    if(size == 0)
    {
        return "";
    }
    //概率性触发事件
    int i = rand()% 100;
    if(i<60)
    {
        return "";
    }
    i = rand() % size;
    QString message = (this->*events[i])();
    events.erase(events.begin() + i);
    return message;
}

//事件0,挖矿带动显卡价格提升
QString my_event::custom_event_0()
{
    int i = s->search_item("xtx 3060");
    s->items[i].current_price *= 3;
    QString message = "受虚拟货币大涨影响，全球挖矿热潮兴起，显卡的价格大幅上涨!";
    return message;
}
//事件1，原魔新版本公测，带动手办价格上升
QString my_event::custom_event_1()
{
    int i = s->search_item("原魔手办");
    s->items[i].current_price *= 3;
    QString message = "原魔新版本前瞻直播结束，热度再创新高，带动了原魔手办价格大幅上涨";
    return message;
}
//事件2,三只田鼠被爆出产品有质量问题
QString my_event::custom_event_2()
{
    int i = s->search_item("三只田鼠零食礼包");
    s->items[i].current_price *= 0.3;
    QString message = "三只田鼠品牌被爆出产品质量问题，现在各大媒体炎上，其产品价格大幅下降";
    return message;
}
//事件3,switch
QString my_event::custom_event_3()
{
    int i = s->search_item("xwitch掌机");
    s->items[i].current_price *= 3;
    QString message = "次世代大作奥尔达在xwitch掌机发布，带动其价格大幅上涨";
    return message;
}
//事件4,猪瘟
QString my_event::custom_event_4()
{
    int i = s->search_item("新鲜猪肉");
    s->items[i].current_price *= 0.3;
    QString message = "新型猪瘟疫爆发，消费者不敢再购买新鲜猪肉，其价格大幅下降";
    return message;
}
//事件5,恋爱节拍器
QString my_event::custom_event_5()
{
    int i = s->search_item("《恋爱节拍器》");
    s->items[i].current_price *= 3;
    QString message = "你所热爱的，就是你的生活，路人女主在睿站的热播带动了其衍生作品《恋爱节拍器》的价格上涨";
    return message;
}
