#ifndef IMPROVE_STATUS_H
#define IMPROVE_STATUS_H
#include <QString>

class improve_status
{
public:
    improve_status();
    improve_status(int number_package, int number_move, int number_market, int number_buy_number);

    //以下为提升属性的购买量
    int number_package;
    int number_move;
    int number_market;
    int number_buy_number;

    //提升属性的表述
    QString str_status[4];//第一个是package，第二个是move，第三个是number_market，第四个为buy_number物品的购买数量
};

#endif // IMPROVE_STATUS_H
