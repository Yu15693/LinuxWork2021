#ifndef BANK_H
#define BANK_H
#include "game_status.h"
#include <QObject>
#include <QTableWidgetItem>
class Bank : public game_status
{
    Q_OBJECT
public:
    explicit Bank(QObject *parent = nullptr);

    ////以下为银行函数
    int can_loan_number = 10000;//能够贷款的数量
    int need_loan_number = 0;//需要还款数量
    QString flagloan;//这是一个标志，表示未进行选择贷款时，贷款按钮将return不做动作
    QString flagrepay;
    void write_to_loan();//进入贷款界面
    void loan_money();//通过贷款的按键，实行贷款
    void repay_money();//还款按钮
    void select_loan(QTableWidgetItem *tableitem);//选择贷款时候的显示文本
    void select_repay(QTableWidgetItem *tableitem);//选择还款时候的显示文本

signals:

};

#endif // BANK_H
