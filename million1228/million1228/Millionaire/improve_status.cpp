#include "improve_status.h"

improve_status::improve_status()
{

}

//初始化可购买数量
improve_status::improve_status(int number_package, int number_move, int number_market, int number_buy_number)
{
    this->number_package = number_package;
    this->number_move = number_move;
    this->number_market = number_market;
    this->number_buy_number = number_buy_number;

    this->str_status[0] = "介绍:扩大10格仓库容量,您可以存放更多的货物.";
    this->str_status[1] = "介绍:提升每天的行动点数,每天执行的行动增加.";
    this->str_status[2] = "介绍:提高市场的分析能力,提升投资涨幅的概率.";
    this->str_status[3] = "介绍:提高每天购买产品的数量，每个商品数量提高百分之十";
}
