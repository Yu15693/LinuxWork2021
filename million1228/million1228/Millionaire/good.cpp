#include "good.h"

good::good()
{

}

good::good(int _id, QString _name, int _price, int _buy_number)
{
    id = _id;
    name = _name;
    price = _price;
    current_price = price;
    buy_number = _buy_number;

    can_buy_number = buy_number;
    average = 0;
    sum = 0;
    have_number = 0;
    trend.push_back(1);
    trend.push_back(1);
}

void good::buy(int number)
{
    sum += current_price * number;
    can_buy_number -= number;
    have_number += number;
    calc_average();
}

void good::sell(int number)
{
    have_number -= number;
    sum = average * have_number;
    //如果商品卖完了，清0平均进货价
    if(have_number == 0)
    {
        average = 0;
    }
}

void good::calc_average()
{
    average = sum / have_number;
}

bool good::twice_decrease()
{
    if(trend[0] < 1 && trend[1] < 1)
        return true;
    return false;
}

bool good::twice_increase()
{
    if(trend[0] > 1 && trend[1] > 1)
        return true;
    return false;
}

void good::inner_change(float scale)
{
    trend[1] = trend[0];
    trend[0] = scale;
    current_price = current_price * scale;
    //如果价格归0，做调整
    if(current_price == 0)
    {
        current_price = 0.2 * price;
    }
}
void good::remain_price()
{
    trend[1] = trend[0];
    trend[0] = 1;
}

//价格算法，1w次后价格在price*1.1左右
void good::change_price()
{
    //有概率不改变价格
    float scale = qrand() % 100;
    if(scale < 30)
    {
        remain_price();
        return;
    }
    bool inc = twice_increase();
    bool dec = twice_decrease();   
    //如果价格连续两次上涨，则这次必跌
    if(inc)
    {
        //控制跌幅
        scale = qrand() % 100;
        while(scale < 40 || scale > 80)
        {
            scale = qrand() % 100;
        }
    }
    //两次跌，这次必涨
    else if(dec)
    {
        //控制涨幅
        scale = qrand() % 200;
        while(scale < 150)
        {
            scale = qrand() % 250;
        }
    }
    //如果当前价格已经超过商品原价格3倍，必跌
    else if(current_price >= price * 3)
    {
        //控制跌幅
        scale = qrand() % 100;
        while(scale < 25 || scale > 60)
        {
            scale = qrand() % 100;
        }
    }
    // 如果当前价格已经小于原价格0.3倍，必涨
    else if(current_price <= price * 0.3)
    {
        //控制涨幅
        scale = qrand() % 300;
        while(scale < 160)
        {
            scale = qrand() % 300;
        }
    }
    //其余情况，正常涨跌
    else
    {
        //控制幅度
        scale = qrand() % 180;
        while(scale < 40)
        {
            scale = qrand() % 180;
        }
    }

    //得到该次幅度
    scale /= 100;
    inner_change(scale);

}
