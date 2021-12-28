#include "game_status.h"
#include <QDebug>
#include <QFile>

game_status::game_status(QObject *parent) : QObject(parent), my_events(this)
{
    v_title_window = true;
    v_game_window = false;
    v_back_btn = false;
    v_main_window = true;
    v_market_window = false;
    v_pakage_window = false;
    v_sell_window = false;
    v_bank_daily = false;
    v_package_button = true;
    v_improve_window = false;

    //yyf
    v_job_window=false;



    day = 1;
    my_money = 10000;
    last_day_money = my_money;
    my_loan = 0;
    action_point = 3;
    now_action_point = action_point;
    total_package = 100;
    package = 0;
    job_level=0;
    job_exp=0;

    //看板娘相关
    wifu_status = "normal";
    is_talk = false;

    //以下为提升
    this->market_status = 0;//市场分析能力，影响投资方式的浮动值
    improveofstatus = improve_status(5, 2, 5, 10);
}
game_status::~game_status()
{
}

//读取文本文件，这里面存储了当前市场售卖的资料
void game_status::read_items()
{
    //打开items文本文件
    QFile file("../data/items.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "读取items文件失败";
        return;
    }
    //获得文本信息
    QString data = file.readAll();
    QStringList lists = data.split("\r\n");//QStringList容器存储
    int length = lists.length();
//    qDebug() << lists.length();

    QStringList infos;//这个再通过","隔开所有物品
    for(int i=0; i<length; i++)
    {
        infos = lists[i].split(",");
        items.push_back(good(infos[0].toInt(), infos[1],
                infos[2].toInt(), infos[3].toInt()));//这个item是一个存储物品ID的数组
    }
    qDebug() << "items[0]" << items[0].id << items[0].name << items[0].price << items[0].buy_number;
}

//改变商品价格
void game_status::current_price_change()
{
//    int a=0;
//    for(int i = 0; i < 100; i++)
//    {
//        for(good &item: items)
//        {

//            item.change_price();
//        }
//        a+= items[10].current_price;
//    }
//    a /= 100;
//    qDebug() << a;
    //对每个商品
    for(good &item: items)
    {
        item.change_price();
    }
    //每天将可购买数回归
    for(good &item: items)
    {
        item.can_buy_number = item.buy_number;
    }
}
//查找某商品下标
int game_status::search_item(QString name)
{
    for(int i = 0; i < items.size(); i++)
    {
        if(items[i].name == name)
        {
            return i;
        }
    }
    //查找不到返回-1
    return -1;
}
//存放着股市的资料
void game_status::read_bank_final()
{
    //打开items文本文件
    QFile file("../data/bankitems.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "读取items文件失败";
        return;
    }
    //获得文本信息
    QString data = file.readAll();
    QStringList lists = data.split("\r\n");//QStringList容器存储
    int length = lists.length();

    QStringList infos;//这个再通过","隔开所有物品
    for(int i = 0; i < length; i++)
    {
        infos = lists[i].split(",");
        bankitems.push_back(Bankfinal(infos[0].toInt(), infos[1], infos[2].toInt(), infos[3].toInt()));
    }
}
//打工
void game_status::read_job_items(){
    QFile file ("../data/jobitems.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"读取item文件失败";
        return ;
    }

    //获取文本信息
    QString data = file.readAll();
    QStringList lists = data.split("\r\n");  //QStringList容器存储
    int length = lists.length();

    QStringList infos;//这个再通过“，”隔开所有物品
    for(int i=0;i<length;i++){
        infos = lists[i].split(",");
        jobitems.push_back(job(infos[0].toInt(),infos[1],infos[2].toInt(),infos[3].toInt(),infos[4].toInt(),infos[5].toInt()));
    }
}





void game_status::loan(int loan_money)
{
    my_loan += loan_money;
    my_money += loan_money;
    loan_number -= loan_money;
}

void game_status::repay_loan(int pay_money)
{
    my_loan -= pay_money;
    my_money -= pay_money;
    can_loan_number += 2 * pay_money;
    if(my_loan == 0)
    {
      loan_number = can_loan_number;
    }
}

void game_status::set_wifu_status(QString status)
{
    wifu_status = status;
}

int game_status::money_situation()
{
    is_talk = false;
    int change = my_money - last_day_money;
    qDebug() << change;
    //happy
    if(change >= 30000)
    {
        set_wifu_status("happy");
        return 1;
    }
    //normal
    if(change >= -10000)
    {
        set_wifu_status("normal");
        return 2;
    }
    //sad
    if(change >= -30000)
    {
        set_wifu_status("sad");
        return 3;
    }
    //anxious
    if(change >= -50000)
    {
        set_wifu_status("anxious");
        return 4;
    }
    //angry
    if(change >= -100000)
    {
        set_wifu_status("angry");
        return 5;
    }
    //very angry
    else
    {
        set_wifu_status("very angry");
        return 6;
    }
}




