#ifndef BASE_WINDOW_H
#define BASE_WINDOW_H

#include <QWidget>
#include "game_status.h"
#include "voiceres.h"
#include <QTableWidget>
#include <QSound>//建立音效输出
#include <QVector>
#include <QStack>
#include <QtGlobal>
#include <QTime>

namespace Ui {
class base_window;
}

class base_window : public QWidget
{
    Q_OBJECT

public:
    explicit base_window(QWidget *parent = 0);
    ~base_window();

    void titleInit();
    void gameInit();
    void window_render();
    void market_table_render();
    void sell_table_render();
    void write_to_Pakage();
    void select_good(QTableWidgetItem *tableitem);
    void give_info(QString info, bool alert = false);
    void select_sell_good(QTableWidgetItem *tableitem);
    void day_pass();

    voiceRes *voiceres;

    //看板娘相关
    void wifu_render();
    void talk_btn_clicked();
    void get_situation_info();
    //以下为银行函数
    void write_to_loan();//进入贷款界面
    void loan_money();//通过贷款的按键，实行贷款
    void repay_money();//还款按钮
    void user_save_money();//用户存款按钮
    void user_get_money();//用户取款按钮
    void bank_final_table();//理财table显示
    void bank_daily();//理财日志显示
    void bank_choose();//选择理财的按钮
    void day_bank_final();//银行投资与时间有关的函数，投资的结算方式
    void select_loan(QTableWidgetItem *tableitem);//选择贷款时候的显示文本
    void select_repay(QTableWidgetItem *tableitem);//选择还款时候的显示文本
    void select_choose_final(QTableWidgetItem *tableitem);//选择理财方式时候的显示文本
    void bank_final_price_change();//投资方式中浮动指数的调整
    void bank_final_deposit_change();//存款因利率变化做的调整
    void bank_daily_write();//日志的书写
    void bank_change();

    ///以下为提升事件
    void improve_buy();//提升属性点购买，提升表格的操作
    void improve_table_select(QTableWidgetItem *tableitem);//提升table的显示


    //打工
    void select_job();//选择工作
    void job_results();//工作结果
    void job_table();//打印工作内容
    void job_table_select(QTableWidgetItem* tableitem);   //选择工作
    void job_level();//工作等级
    int job_exp(int exp);//经验计算函数，用于进度条显示
    //void job_change();  //工作变化   还没写呢


signals:
    void topbar_changed();

public slots:
    void changePlace();
    void change_topbar();
    void buy_btn_clicked();
    void sell_btn_clicked();
    void job_btn_clicked();

private:
    Ui::base_window *ui;
    game_status* s;

private slots:
    void on_start_btn_clicked();
    void on_back_btn_clicked();
    void on_exit_btn_clicked();

};

#endif // BASE_WINDOW_H
