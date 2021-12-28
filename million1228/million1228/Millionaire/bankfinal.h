#ifndef BANKFINAL_H
#define BANKFINAL_H
#include <QString>

class Bankfinal
{
public:
    Bankfinal();
    Bankfinal(int id, QString name, int price, int day);

    int id;//理财方式的id
    QString name;//理财名称
    double price;//浮动指数
    int day;//理财返现的天数
    int money;//预计收入
    int number;//买入时候的价格，用于调整浮动值变化时预计收入的变化
};

#endif // BANKFINAL_H
