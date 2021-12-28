#ifndef GOOD_H
#define GOOD_H

#include <QObject>
#include <QVector>
//商品类头文件
class good
{
public:
    good();
    good(int _id, QString _name, int _price, int _buy_number);

    int id; //商品id
    QString name; //商品名称
    int price; //商品价格
    int current_price; //商品当前价格
    int buy_number; //商品可购买数
    int can_buy_number; //剩余商品可购买数
    int average; //商品平均进货价
    int sum; //商品购入总价值
    int have_number; //拥有商品数量
    QVector<float> trend; //商品升降趋势

    void buy(int number);
    void sell(int number);
    void calc_average();//计算平均值
    //获知是否连续两次增加或减小
    bool twice_increase();
    bool twice_decrease();
    //根据趋势改变商品价格
    void inner_change(float scale);
    void remain_price();
    void change_price();
    good& operator =(const good _good);

};

#endif // GOOD_H
