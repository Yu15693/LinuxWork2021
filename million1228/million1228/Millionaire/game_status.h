#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include <QObject>
#include <Qvector>
#include <QString>
#include "good.h"
#include "bankfinal.h"
#include "my_event.h"
#include "improve_status.h"
#include "job.h"
class game_status : public QObject
{
    Q_OBJECT
public:
    explicit game_status(QObject *parent = nullptr);
    ~game_status();
    bool v_title_window;
    bool v_game_window;
    bool v_back_btn;
    bool v_main_window;
    bool v_market_window;
    bool v_pakage_window;
    bool v_sell_window;
    bool v_bank_window;
    bool v_package_button;
    bool v_bank_daily;
    bool v_improve_window;

    //yyf
    bool v_job_window;



    int day;
    int my_money;
    int last_day_money;
    int my_loan;
    int action_point;
    int now_action_point;
    int total_package;
    int package;
    int job_level;//工作等级
    int job_exp; //工作经验

    int n_items;
    //商品向量
    QVector<good>items;//这个items里面存放了所有货物
    //事件实例
    my_event my_events;
    //看板娘相关
    QString wifu_status;
    void set_wifu_status(QString status);
    int money_situation();
    bool is_talk;
    //读取文本文件
    void read_items();
    //改变商品价格
    void current_price_change();
    //查找某商品下标
    int search_item(QString name);

    //银行
    int can_loan_number = 10000;//最大能够贷款的数量
    int loan_number = 10000; //目前可贷款金额
    double user_bank_deposit = 0;//用户的存款
    double user_bank_deposit_percentage = 1;//用户的存款利率
    QVector<QString> last_day_bank;

    void read_bank_final();//读取股市文本文件
    QVector<Bankfinal> bankitems;//存放着所有投资方式
    QVector<Bankfinal> bankfinal;//被选择的投资方式

    //打工
    void read_job_items();   //从文件内读取工作信息
    QVector<job> jobitems;   //存放着所有的工作信息




    void loan(int loan_money);
    void repay_loan(int pay_money);

    ///以下为提升
    double market_status;//市场分析能力，影响投资方式的浮动值
    improve_status improveofstatus;
signals:
public slots:

};

#endif // GAME_STATUS_H
