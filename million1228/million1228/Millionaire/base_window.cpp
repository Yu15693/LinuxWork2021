#include "base_window.h"
#include "ui_base_window.h"
#include <QDebug>
#include <QPainter>
#include <QPushButton>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QMouseEvent>
#include <QMessageBox>
#include <QProgressBar>

base_window::base_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::base_window)
{
    ui->setupUi(this);

    voiceres = new voiceRes;//音效初始化
    titleInit();//标题头部文件初始化
}

base_window::~base_window()
{
    delete ui;
    delete s;
    delete voiceres;
}



void base_window::window_render()//读取gamestatus下的成员
{
    ui->title_window->setVisible(s->v_title_window);//最初的标题界面
    ui->game_window->setVisible(s->v_game_window);//返回和仓库的操控界面
    ui->back_btn->setVisible(s->v_back_btn);//返回按钮
    ui->main_window->setVisible(s->v_main_window);//行动界面
    ui->market_window->setVisible(s->v_market_window);//市场界面
    ui->pakage_window->setVisible(s->v_pakage_window);//仓库界面
    ui->sell_window->setVisible(s->v_sell_window);//出售界面
    ui->bank_window->setVisible(s->v_bank_window);//银行界面
    ui->package_btn->setVisible(s->v_package_button);//仓库按钮
    ui->dailywindow->setVisible(s->v_bank_daily);//银行日志界面
    ui->improve_window->setVisible(s->v_improve_window);//提升界面


    //工作
    ui->job->setVisible(s->v_job_window);




}

void base_window::titleInit()//初始化头部文件
{
    s = new game_status(this);
    window_render();
}

void base_window::gameInit()
{
    //要改出去
    //读取商品信息
    s->read_items();//读取文本文件，这里面存储了当前市场售卖的资料
    s->read_bank_final();
    s->read_job_items();  //读取文本文件， 里面存储了当前可选的工作

    //获取游戏主界面窗口下所有点击按钮，与事件相连接
    QList<QPushButton *> btns = ui->main_window->findChildren<QPushButton *>();
    foreach (auto btn, btns)
    {
        connect(btn, &QPushButton::clicked, this, &base_window::changePlace);
        connect(btn, &QPushButton::clicked, voiceres, &voiceRes::give_voice_startgame);
    }
    //表格变化,这里是选取表格形式，不可编辑的状态
    //选取所有游戏窗口内的表格
    QList<QTableWidget *> tables = ui->game_window->findChildren<QTableWidget *>();
    foreach (auto table, tables) {
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    //仓库按钮连接
    //仓库界面置于最顶层
    ui->package_btn->raise();
    connect(ui->package_btn, &QPushButton::clicked, this, &base_window::changePlace);
    //顶部UI连接信号
    connect(this, &base_window::topbar_changed, this, &base_window::change_topbar);

    //出售界面
    connect(ui->sell_table, &QTableWidget::itemClicked, this, &base_window::select_sell_good);
    ui->item_name_label->setProperty("id", -1);
    ui->sell_number_edit->setText("0");
    ui->sell_number_edit->setValidator(new QIntValidator(0, 999, this));
    //连接出售按钮
    connect(ui->sell_items_btn, &QPushButton::clicked, this, &base_window::sell_btn_clicked);


    //银行信号槽
    QList<QPushButton *> btns1 = ui->bank_window->findChildren<QPushButton *>();
    foreach (auto btn, btns1)
       connect(btn, &QPushButton::clicked, voiceres, &voiceRes::give_voice_buygood);
    connect(ui->bank_btn, &QPushButton::clicked, this, &base_window::write_to_loan);//贷款按钮
    connect(ui->bank_table, &QTableWidget::itemClicked, this, &base_window::select_repay);//银行表格事件源
    connect(ui->bank_table_finantial, &QTableWidget::itemClicked, this, &base_window::select_choose_final);//理财表格事件源
    connect(ui->bank_button_loan, &QPushButton::clicked, this, &base_window::loan_money);//贷款按钮
    connect(ui->bank_button_repay, &QPushButton::clicked, this, &base_window::repay_money);//还款按钮
    connect(ui->bank_button_deposit, &QPushButton::clicked, this, &base_window::user_save_money);//存款按钮
    connect(ui->bank_buttton_getmoney, &QPushButton::clicked, this, &base_window::user_get_money);//取款按钮
    connect(ui->bank_button_daily, &QPushButton::clicked, this, &base_window::bank_daily);//日志按钮
    connect(ui->bank_button_choose, &QPushButton::clicked, this, &base_window::bank_choose);//理财按钮
    ui->item_name_repay->setProperty("id", -1);
    ui->item_name_choose->setProperty("id", -1);

    //提升信号槽
    connect(ui->improve_status_btn, &QPushButton::clicked, this, &base_window::improve_buy);//提升按键，购买属性点
    connect(ui->improve_table, &QTableWidget::itemClicked, this, &base_window::improve_table_select);//提升属性点表格事件源
    ui->improve_table->setProperty("id", -1);

    //打工信号槽
    connect(ui->job_table,&QTableWidget::itemClicked,this,&base_window::job_table_select);
    connect(ui->select_job,&QPushButton::clicked,this,&base_window::job_btn_clicked);//点击选择按钮选择工作
    ui->job_table->setProperty("id",-1);

    QList<QLineEdit *> bankedits = ui->bank_window->findChildren<QLineEdit *>();
    foreach(auto bankedit, bankedits)
    {
        bankedit->setText("0");
        bankedit->setValidator(new QIntValidator(0, 1000000, this));
    }

    //市场界面
    //连接市场表格
    connect(ui->market_table, &QTableWidget::itemClicked, this, &base_window::select_good);
    //购买栏及编辑栏
    ui->item_name_label->setProperty("id", -1);
    ui->buy_number_edit->setText("0");
    ui->buy_number_edit->setValidator(new QIntValidator(0, 999, this));
    //购买按钮连接
    connect(ui->buy_btn, &QPushButton::clicked, this, &base_window::buy_btn_clicked);
    market_table_render();

    //显示顶部信息
    change_topbar();
    //银行初始改变汇率
    bank_final_price_change();
    //窗口显示
    window_render();
}
//游戏主界面的按钮点击事件
void base_window::changePlace()
{
    QPushButton* btn= qobject_cast<QPushButton *>(sender());
    int type = btn->property("place_type").toInt();
    switch (type) {
    case 0:
        //按钮触发仓库事件
        if(s->v_pakage_window)
        {
            s->v_pakage_window = false;
            if(s->v_main_window != true)
            {
                s->v_back_btn = true;
            }
            ui->package_btn->setStyleSheet("text-align: left;");
        }
        else
        {
            write_to_Pakage();
            s->v_back_btn = false;
            s->v_pakage_window = true;
            ui->package_btn->setStyleSheet("background-color: #A89393; text-align: left;");
        }

        break;
    case 1:
        //按钮批发市场事件
        if(s->now_action_point == 0)
        {
            give_info("您已经没有行动点数了",true);
            return;
        }
        s->v_market_window = true;
        s->v_main_window = false;
        s->v_back_btn = true;
        market_table_render();
        s->now_action_point--;
        break;
    case 2:
        //按钮触发出售窗口事件
        s->v_sell_window = true;
        s->v_main_window = false;
        s->v_back_btn = true;
        sell_table_render();
        break;
    case 3:
        //打工事件
        s->v_job_window = true;
        s->v_main_window = false;
        s->v_back_btn = true;
        job_table();
        break;
    case 4:
        //银行事件4
        if(s->now_action_point == 0)
        {
            give_info("您已经没有行动点数了",true);
            return;
        }
        write_to_loan();
        bank_daily_write();
        s->v_bank_window = true;
        s->v_main_window = false;
        s->v_back_btn = true;
        s->now_action_point--;
        break;
    case 5:
        //提升事件
        s->v_improve_window = true;
        s->v_main_window = false;
        s->v_back_btn = true;
        break;
    case 6:
        //点击休息按钮
        day_pass();
        break;
    case 7:
        //点击交谈
        talk_btn_clicked();
        break;
    default:
        break;
    }
    window_render();
    topbar_changed();
}

void base_window::change_topbar()//顶部的UI界面,这个函数连接了主窗口的信号
{
    ui->topbar->setText(QString("今天是第%1天,金钱:%2，待还贷款:%3\n行动点数：%4 仓库容量：%5/%6 ，今天也继续努力吧！")
                        .arg(s->day, 2, 10, QLatin1Char(' '))
                        .arg(s->my_money, 7, 10, QLatin1Char(' '))
                        .arg(s->my_loan, 7, 10, QLatin1Char(' '))
                        .arg(s->now_action_point, 1, 10, QLatin1Char(' '))
                        .arg(s->package, 4, 10, QLatin1Char(' '))
                        .arg(s->total_package, 4, 10, QLatin1Char(' ')));
}

void base_window::market_table_render()//购买成功之后，重新改变当前这个弹窗的数值
{
    ui->market_table->clearContents();
    ui->market_table->setRowCount(s->items.length());
    for(int i=0; i<s->items.length(); i++)
    {
        ui->market_table->setItem(i,0,new QTableWidgetItem(QString::number(s->items[i].id)));
        ui->market_table->setItem(i,1,new QTableWidgetItem(s->items[i].name));
        ui->market_table->setItem(i,2,new QTableWidgetItem(QString::number(s->items[i].current_price)));
        ui->market_table->setItem(i,3,new QTableWidgetItem(QString::number(s->items[i].can_buy_number)));
    }
}

void base_window::select_good(QTableWidgetItem *tableitem)//每个table表格中点击返回信号源
{
    qDebug() << tableitem->row() << tableitem->column();
    int id = tableitem->row();
    ui->item_name_label->setText(QString("商品:").append(s->items[id].name));
    ui->item_name_label->setProperty("id", id);//给予了一个标志
}

//出售的表格选择物品
void base_window::select_sell_good(QTableWidgetItem *tableitem)
{
    qDebug() << tableitem->row() << tableitem->column();
    int id = tableitem->row();
    ui->sell_item_name_label->setText(QString("商品:").append(s->items[id].name));
    ui->sell_item_name_label->setProperty("id", id);//给予了一个标志
}

//点击出售按钮
void base_window::sell_btn_clicked()
{
    int number = ui->sell_number_edit->text().toInt();
    int id = ui->sell_item_name_label->property("id").toInt();
    //没选择物品或没有购买数量时直接返回
    if(id == -1 || number ==0)
    {
        return;
    }
    if(number > s->items[id].have_number)
    {
        give_info("商品剩余数量不足！", true);
        return;
    }
    //出售成功
    int money = s->items[id].current_price * number;
    s->items[id].sell(number);
    s->package -= number;
    s->my_money += money;
    give_info("出售" + s->items[id].name +QString::number(number)+"个成功！", false);
    //渲染界面
    sell_table_render();
    change_topbar();
    voiceres->give_voice_buygood();//音效的加入
}

void base_window::buy_btn_clicked()//点击购买的按键
{
    int number = ui->buy_number_edit->text().toInt();
    int id = ui->item_name_label->property("id").toInt();

    qDebug() << id << number;
    //没选择物品或没有购买数量时直接返回
    if(id == -1 || number ==0)
    {
        return;
    }
    if(number > s->items[id].can_buy_number)
    {
        give_info("购买量过大！", true);
        return;
    }
    if(number + s->package > s->total_package)
    {
        give_info("仓库容量不足", true);
        return;
    }
    int spend = s->items[id].current_price * number;
    if(spend > s->my_money)
    {
        give_info("剩余金钱不足", true);
        return;
    }
    //购买成功

    s->package += number;
    s->my_money -= spend;
    s->items[id].buy(number);
    give_info("购买" + s->items[id].name +QString::number(number)+"个成功！", false);
    //渲染界面
    market_table_render();
    change_topbar();
    voiceres->give_voice_buygood();//音效的加入
}

void base_window::give_info(QString info, bool alert)//显示出一个文本，像一个弹窗
{
    if(alert)
    {
        ui->info_label->setStyleSheet("background-color: #D8D8D8;color: red;");
    }
    else
    {
        ui->info_label->setStyleSheet("background-color: #D8D8D8;");
    }
    ui->info_label->setText(info);
}

//点击开始按钮触发游戏初始化
void base_window::on_start_btn_clicked()
{
    voiceres->give_voice_startgame();
    s->v_title_window = false;
    s->v_game_window = true;
    gameInit();
}

//关闭按钮关闭主窗口
void base_window::on_exit_btn_clicked()
{
    this->close();
}

//返回按钮使main窗口可见，其余全不可见
void base_window::on_back_btn_clicked()
{
    s->v_main_window = true;
    s->v_market_window = false;
    s->v_back_btn = false;
    s->v_pakage_window = false;
    s->v_bank_window = false;
    s->v_package_button = true;
    s->v_sell_window = false;
    s->v_improve_window = false;
    s->v_job_window = false;
    window_render();
}

void base_window::write_to_Pakage()//每次点击仓库的时候，他将显示item内物品的数目
{
    ui->pakage_table->clearContents();
    ui->pakage_table->setRowCount(s->items.length());
    for(int i = 0; i < s->items.length(); i++)
    {
        ui->pakage_table->setItem(i, 0, new QTableWidgetItem(QString::number(s->items[i].id)));
        ui->pakage_table->setItem(i, 1, new QTableWidgetItem(s->items[i].name));
        ui->pakage_table->setItem(i, 2, new QTableWidgetItem(QString::number(s->items[i].average)));
        ui->pakage_table->setItem(i, 3, new QTableWidgetItem(QString::number(s->items[i].have_number)));
    }
}

//渲染出售界面的表格
void base_window::sell_table_render()
{
    ui->sell_table->clearContents();
    ui->sell_table->setRowCount(s->items.length());
    for(int i = 0; i < s->items.length(); i++)
    {
        ui->sell_table->setItem(i, 0, new QTableWidgetItem(QString::number(s->items[i].id)));
        ui->sell_table->setItem(i, 1, new QTableWidgetItem(s->items[i].name));
        ui->sell_table->setItem(i, 2, new QTableWidgetItem(QString::number(s->items[i].current_price)));
        ui->sell_table->setItem(i, 3, new QTableWidgetItem(QString::number(s->items[i].have_number)));
    }
}

//休息按钮按下，过一天
void base_window::day_pass()
{
    qsrand(time(NULL));
    s->day++;

    if(s->my_money - s->my_loan >= 1000000)
    {
        QMessageBox::information(this, "游戏胜利", QString("恭喜你，你已经挣到了1000000元以上！"));
        this->close();
        return;
    }
    if(s->day > 30)
    {
        QMessageBox::information(this, "游戏失败", QString("可惜了，30天内您的金额没有超过1000000元"));
        return;
    }
    s->now_action_point = s->action_point;
    //价格变化
    s->current_price_change();
    //银行相关变化
    bank_change();
    QString message = s->my_events.trigger_event();
    get_situation_info();
    change_topbar();
    QMessageBox::information(this, "起床了", QString("今天是第%1天, 今日消息如下：\n")
                             .arg(s->day, 2, 10, QLatin1Char(' '))
                             .append(message));

    s->last_day_money = s->my_money;
}

void base_window::bank_change()//有关银行变化的函数
{
    ////银行随着天数增加时候的操作
    bank_final_price_change();//浮动随机数
    bank_daily_write();//日志的书写
    day_bank_final();//投资结算函数
    bank_final_deposit_change();//存款因利率的变化

    change_topbar();
}

////以下为控制银行的函数,操作game_status对象s,s对象中由我的贷款
void base_window::bank_daily_write()//日志，能够查看当前正在进行的投资和结算时候的获得钱数
{
    ui->listWindow->clear();//清空list
    ui->listWindow->addItem(QString("投资名称，返现天数，购买本金，预计收入"));
    for(QVector<Bankfinal>::iterator it = s->bankfinal.begin(); it != s->bankfinal.end(); it++)
    {
        if(it->day > 0)//不显示已经结算过的投资，天数未到的时候才显示
        {
        it->money = it->money + (s->bankitems[it->id - 1].price * it->money);//当前投资期望得到的金钱
        ui->listWindow->addItem(QString("%1\t%2             %3           %4").arg(it->name).arg(it->day).arg(it->number).arg(it->money));
        }
    }
}

void base_window::bank_final_price_change()//浮动随机数，存款利率的随机，投资浮动的随机，投资天数返现的随机
{
    QVector<Bankfinal>::iterator it;
    s->user_bank_deposit_percentage = rand() / double(RAND_MAX);//当前存款的利率
    for(it = s->bankitems.begin(); it != s->bankitems.end(); it++)
    {
        switch (it->id)//优化每个投资方式的变化
        {
        case 1://投资
            it->day = qrand() % 5 + 1;
            //it->price =(rand() / double(RAND_MAX) + (qrand() % 1)) + s->market_status;
            it->price = rand() / double(RAND_MAX) - rand() / double(RAND_MAX) + s->market_status;
            break;
        case 2://存定期
            it->day = 5;
            //it->price = qrand() % 2 * 3 + s->market_status;
            it->price = rand() / double(RAND_MAX) / 10;
            break;
        case 3://投期货
            it->day = qrand() % 10 + 1;
            //it->price = (rand() / double(RAND_MAX) + (qrand() % 1)) * 10 + s->market_status * 2;
            it->price = rand() / double(RAND_MAX) - rand() / double(RAND_MAX) + s->market_status;
            break;
        case 4://炒股
            it->day = 1;
            //it->price = (rand() / double(RAND_MAX) + (qrand() % 10 - 5)) * 5 + s->market_status * 5;
            it->price = rand() / double(RAND_MAX) - rand() / double(RAND_MAX) + + s->market_status;
            break;
        case 5:
            it->day = qrand() % 10 + 10;
            //it->price = (rand() / double(RAND_MAX) + (qrand() % 4 - 2) * 3) + qrand() % 4 - 2 + s->market_status * 5;
            it->price = rand() / double(RAND_MAX) - rand() / double(RAND_MAX) + + s->market_status;
            break;
        default:
            break;
        }
        qDebug() << it->price;
    }
}

void base_window::bank_final_deposit_change()//存款变化
{
    s->user_bank_deposit += (s->user_bank_deposit * s->user_bank_deposit_percentage / 100);
}

void base_window::day_bank_final()//投资结算的函数
{
    s->last_day_bank.clear();//清空昨天的数组
    int numofzero = 0;//结算的个数
    QVector<Bankfinal>::iterator it;
    for(it = s->bankfinal.begin(); it != s->bankfinal.end(); it++)
    {
        it->day--;//id存放的是投资返现的天数
        if(it->day == 0)
        {
            s->my_money += it->money;
            //
            s->last_day_bank.push_back(QString("%1结算: %2").arg(it->name).arg(it->money));
            give_info(QString("%1结算").arg(it->name), true);
        }
    }
    for(int i = 0; i < numofzero; i++)
        s->bankfinal.erase(s->bankfinal.begin());
}

void base_window::bank_choose()//选择投资方式
{
    int number = ui->edit_number_choose->text().toInt();
    int id = ui->item_name_choose->property("id").toInt();
    if(id == -1 || number == 0)
    {
        return;
    }
    s->bankitems[id].number = number;
    s->bankitems[id].money = number + number * s->bankitems[id].price;
    s->my_money -= number;
    ui->listWindow->addItem(QString("%1\t%2             %3           %4").arg(s->bankitems[id].name)
                            .arg(s->bankitems[id].day).arg(s->bankitems[id].number).arg(s->bankitems[id].money));
    s->bankfinal.push_back(s->bankitems[id]);
    change_topbar();
}

void base_window::bank_daily()//打开和关闭日志
{
    if(s->v_bank_daily)
    {
        s->v_bank_daily = false;
        ui->bank_button_daily->setStyleSheet("");
    }
    else
    {
        s->v_bank_daily = true;
        ui->bank_button_daily->setStyleSheet("background-color: #A89393;");
    }
     window_render();
}
void base_window::bank_final_table()//打印投资table中的信息
{
    ui->bank_table_finantial->clearContents();
    ui->bank_table_finantial->setRowCount(s->bankitems.length());
    for(int i=0; i<s->bankitems.length(); i++)
    {
    ui->bank_table_finantial->setItem(i,0,new QTableWidgetItem(QString::number(s->bankitems[i].day)));
    ui->bank_table_finantial->setItem(i,1,new QTableWidgetItem(s->bankitems[i].name));
    ui->bank_table_finantial->setItem(i,2,new QTableWidgetItem(QString::number(s->bankitems[i].price)));
    }
}


void base_window::job_table(){
    ui->job_table->clearContents();
    ui->job_table->setRowCount(s->jobitems.length());
    for(int i = 0 ; i<s->jobitems.length();i++){
        ui->job_table->setItem(i,0,new QTableWidgetItem(QString::number(s->jobitems[i].id)));
        ui->job_table->setItem(i,1,new QTableWidgetItem(s->jobitems[i].job_name));
        ui->job_table->setItem(i,2,new QTableWidgetItem(QString::number(s->jobitems[i].job_exp)));
        ui->job_table->setItem(i,3,new QTableWidgetItem(QString::number(s->jobitems[i].action_point_cost)));
        ui->job_table->setItem(i,4,new QTableWidgetItem(QString::number(s->jobitems[i].job_price)));
        ui->job_table->setItem(i,5,new QTableWidgetItem(QString::number(s->jobitems[i].job_level)));
    }
    //ui->job_level->setText(QString("工作等级:").append(QString::number(s->job_level)));
    job_level();
}

void base_window::job_level(){
    //ui->job_level_exp->setParent(this);
    ui->job_level_exp->setValue(job_exp(s->job_exp));
    ui->job_level->setText(QString("工作等级:").append(QString::number(s->job_level)));
}



void base_window::write_to_loan()//打印函数，打印银行中的所有信息
{
    //贷款打印,还款打印
    qDebug() << "loan";
    ui->bank_table->clearContents();
    ui->bank_table->setRowCount(1);
    ui->bank_table->setItem(0, 0, new QTableWidgetItem(QString("小贷")));
    ui->bank_table->setItem(0, 1, new QTableWidgetItem(QString::number(s->loan_number)));
    ui->bank_table->setItem(0, 2, new QTableWidgetItem(QString::number(s->my_loan)));
    //存款打印
    ui->bank_deposit->clearContents();
    ui->bank_deposit->setRowCount(1);
    ui->bank_deposit->setItem(0, 0, new QTableWidgetItem(QString("用户：")));
    ui->bank_deposit->setItem(0, 1, new QTableWidgetItem(QString::number(s->user_bank_deposit)));
    ui->bank_deposit->setItem(0, 2, new QTableWidgetItem(QString::number(s->user_bank_deposit_percentage)));
    //理财产品打印
    bank_final_table();
}

void base_window::select_choose_final(QTableWidgetItem *tableitem)//显示选的投资方式
{
    qDebug() << tableitem->row() << tableitem->column();
    int id = tableitem->row();
    ui->item_name_choose->setText(QString("理财方式:").append(s->bankitems[id].name));
    ui->item_name_choose->setProperty("id", id);//给予了一个标志
}

void base_window::select_loan(QTableWidgetItem *tableitem)//显示选择的贷款
{
    qDebug() << tableitem->row() << tableitem->column();
    int id = tableitem->row();
    ui->item_name_loan->setText(QString("贷款:").append("小贷"));
    ui->item_name_loan->setProperty("id",id);
}

void base_window::select_repay(QTableWidgetItem *tableitem)//显示选择的还款
{
    qDebug() << tableitem->row() << tableitem->column();
    int id = tableitem->row();
    ui->item_name_repay->setText(QString("贷款:").append("小贷"));
    ui->item_name_repay->setProperty("id",id);
}

void base_window::user_get_money()//取钱的函数
{
    int number = ui->edit_number_deposit->text().toInt();
    if(number > s->user_bank_deposit)
    {
        give_info("您的存款不够", true);
        return;
    }
    s->my_money += number;
    s->user_bank_deposit -= number;
    voiceres->give_voice_buygood();//音效的加入
    write_to_loan();
    change_topbar();
}

void base_window::user_save_money()//存钱的函数
{
    int number = ui->edit_number_deposit->text().toUInt();
    if(number > s->my_money)
    {
        give_info("金钱余额不足以存款", true);
        return;
    }
    s->my_money -= number;
    s->user_bank_deposit += number;
    voiceres->give_voice_buygood();//音效的加入
    write_to_loan();
    change_topbar();
}

void base_window::repay_money()//还款的函数
{
    int number = ui->loan_number_edit->text().toInt();
    int id = ui->item_name_repay->property("id").toInt();
    if(number == 0 || id == -1)
    {
        return;
    }
    if(number > s->my_loan)
    {
        number = s->my_loan;
    }
    s->repay_loan(number);
    voiceres->give_voice_buygood();//音效的加入
    this->write_to_loan();
    this->change_topbar();
}

void base_window::loan_money()//贷款的函数
{
    int number = ui->loan_number_edit->text().toInt();
    int id = ui->item_name_repay->property("id").toInt();
    if(number == 0 || id == -1 )
    {
        return;
    }
    else if(number > this->s->loan_number)
    {
        give_info("超过可贷金额!", true);
        return;
    }
    s->loan(number);
    this->write_to_loan();
    this->change_topbar();
    voiceres->give_voice_buygood();//音效的加入
}

void base_window::wifu_render()
{
    QString status = s->wifu_status;
    QString style = "background: transparent; border-image: url(:/wifu/img/wifu/" + status + ".png);";
    ui->wifu_label->setStyleSheet(style);
}

void base_window::talk_btn_clicked()
{
    if(s->is_talk == false)
    {
        s->set_wifu_status("normal");
        s->is_talk = true;
    }
    if(s->wifu_status == "happy")
    {
        s->set_wifu_status("normal");
        ui->talk_info_label->setText("嘟嘟噜~~");
    }
    else if(s->wifu_status == "normal")
    {
        s->set_wifu_status("happy");
        ui->talk_info_label->setText("不要气馁哦，用心经营，一定会有所收获的");
    }

    wifu_render();
}

void base_window::get_situation_info()
{
    int status = s->money_situation();
    QString review;
    switch (status) {
    case 1:
        review ="每天都在踏踏实实挣钱呢";
        break;
    case 2:
        review ="还算经营的马马虎虎吧";
        break;
    case 3:
        review ="挨，钱少了一点";
        break;
    case 4:
        review ="稍微有点担心呢";
        break;
    case 5:
        review ="一定要慎重花钱哦";
        break;
    case 6:
        review ="哎呀，注意开销！！";
        break;
    default:
        break;
    }
    QString trends;
    for(int i = 0, j = 0; j < 10 && i < s->items.size(); i++)
    {
        QString a = "商品:";
        a += s->items[i].name;
        a += " 的价格";
        float trend = s->items[i].trend[0];
        if(trend > 1.6)
        {
            a += "大幅上涨";
        }
        else if(trend > 1)
        {
            a += "上涨";
        }
        else if(trend == 1)
        {
            continue;
        }
        else if(trend > 0.6)
        {
            a += "下降";
        }
        else
        {
            a += "大幅下降";
        }
        a += "\n";
        j++;
        trends += a;

    }
//    QVector<Bankfinal>::iterator it;
//    for(it = s->bankfinal.begin(); it != s->bankfinal.end(); it++)
//    {
//        it->day--;//id存放的是投资返现的天数
//        if(it->day == 0)
//        {
//            s->my_money += it->money;
//            //
//            s->last_day_bank.push_back(QString("%1结算: %2").arg(it->name).arg(it->money));
//            give_info(QString("%1结算").arg(it->name), true);
//        }
//    }
    QString str_last_bank = "";//输出上一天投资状况
    if(!s->last_day_bank.empty())
    for(QVector<QString>::iterator it = s->last_day_bank.begin(); it!= s->last_day_bank.end(); it++)
        str_last_bank.append(it).append("\n");
    else
        str_last_bank.append("无");
    ui->talk_info_label->setText(QString("今天是第%1天\n与昨天的金钱变化为:%2\n")
                        .arg(s->day, 2, 10, QLatin1Char(' '))
                        .arg(s->my_money - s->last_day_money, 7, 10, QLatin1Char(' '))
                        .append(review)
                        .append("\n今日的部分商品价格走势如下:\n")
                        .append(trends)
                        .append("昨日银行投资收支状况:\n")
                        .append(str_last_bank));
    wifu_render();
}

///以下为提升操控函数
void base_window::improve_buy()//提升购买属性点
{
    int id = ui->improve_table->property("id").toInt();
    if( id == -1)
        return;
    switch (id)
    {
    case 0 ://购买仓库容量
        if(s->my_money < ui->improve_table->item(0, 2)->text().toInt())
        {
            give_info("金额不足", true);
             return;
        }
        if(s->improveofstatus.number_package > 0)
        {
            s->my_money -= ui->improve_table->item(0, 2)->text().toInt();
            s->improveofstatus.number_package--;
            s->total_package += 50;
            ui->improve_table->setItem(0, 2, new QTableWidgetItem(QString::number(ui->improve_table->item(0, 2)->text().toInt() + 20000)));
            if(s->improveofstatus.number_package == 0)
            {
                ui->improve_table->setItem(0, 2, new QTableWidgetItem(QString::number(0)));
                ui->improve_table->setItem(0, 3, new QTableWidgetItem(QString("不可购买")));
            }
        }
        else
        {
            give_info("仓库已最大", true);
        }
        break;
    case 1://购买行动点数
        if(s->my_money < ui->improve_table->item(1, 2)->text().toInt())
        {
            give_info("金额不足", true);
            return;
        }
        if(s->improveofstatus.number_move > 0)
        {
            s->my_money -= ui->improve_table->item(1, 2)->text().toInt();
            s->improveofstatus.number_move--;
            s->action_point++;
            ui->improve_table->setItem(1, 2, new QTableWidgetItem(QString::number(ui->improve_table->item(1, 2)->text().toInt() + 100000)));
            if(s->improveofstatus.number_move == 0)
            {
                ui->improve_table->setItem(1, 2, new QTableWidgetItem(QString::number(0)));
                ui->improve_table->setItem(1, 3, new QTableWidgetItem(QString("不可购买")));
            }
        }
        else
        {
            give_info("行动点已最大", true);
        }
        break;
    case 2:
        if(s->my_money < ui->improve_table->item(2, 2)->text().toInt())
        {
            give_info("金额不足", true);
            return;
        }
        if(s->improveofstatus.number_market > 0)
        {
            s->my_money -= ui->improve_table->item(2, 2)->text().toInt();
            s->improveofstatus.number_market--;
            s->market_status += 0.2;
            ui->improve_table->setItem(2, 2, new QTableWidgetItem(QString::number(ui->improve_table->item(2, 2)->text().toInt() + 30000)));
            if(s->improveofstatus.number_market == 0)
            {
                ui->improve_table->setItem(2, 2, new QTableWidgetItem(QString::number(0)));
                ui->improve_table->setItem(2, 3, new QTableWidgetItem(QString("不可购买")));
            }
        }
        else
        {
            give_info("市场分析能力已最大", true);
        }
        break;
    case 3:
        if(s->my_money < ui->improve_table->item(3, 2)->text().toInt())
        {
            give_info("金额不足", true);
            return;
        }
        if(s->improveofstatus.number_buy_number > 0)
        {
            s->my_money -= ui->improve_table->item(3, 2)->text().toInt();
            s->improveofstatus.number_buy_number--;
             ui->improve_table->setItem(3, 2, new QTableWidgetItem(QString::number(ui->improve_table->item(3, 2)->text().toInt() + 20000)));
            for(QVector<good>::iterator it = s->items.begin(); it != s->items.end(); it++)
                it->buy_number = it->buy_number + it->buy_number * 0.3;
            if(s->improveofstatus.number_buy_number == 0)
            {
                ui->improve_table->setItem(3, 2, new QTableWidgetItem(QString::number(0)));
                ui->improve_table->setItem(3, 3, new QTableWidgetItem(QString("不可购买")));
            }
        }
        else
        {
            give_info("扩大购买量能力已最大", true);
        }
    default:
        break;
    }
    change_topbar();
    voiceres->give_voice_buygood();//音效的加入
}

void base_window::improve_table_select(QTableWidgetItem *tableitem)//提升table显示
{
    qDebug() << tableitem->row() << tableitem->column();
    int id = tableitem->row();
    ui->improve_itemname_label->setGeometry(QRect(770, 120, 329, 27*4));  //四倍行距
    ui->improve_itemname_label->setWordWrap(true);
    ui->improve_itemname_label->setText(s->improveofstatus.str_status[id]);
    ui->improve_itemname_label->adjustSize();
    ui->improve_id_label->setText(QString("您已选择:%1").arg(ui->improve_table->item(id, 1)->text()));
    ui->improve_table->setProperty("id", id);
}



//打工函数

void base_window::job_table_select(QTableWidgetItem *tableitem){//选择工作
    qDebug()<<tableitem->row()<<tableitem->column();
    int id = tableitem->row();
    ui->job_table->setProperty("id",id);
}


void base_window::job_btn_clicked(){

    int id = ui->job_table->property("id").toInt();
    qDebug()<<id;
    if(id==-1){
        return;
    }
    //等级不够
    if(s->jobitems[id].job_level>s->job_level){
        give_info("等级过低，无法选择这项工作",true);
    }
    else if(s->jobitems[id].action_point_cost>s->now_action_point){
        give_info("行动点数不足，无法选择这项工作",true);
    }
    else{
        int salary = s->jobitems[id].job_price;      //增加薪水
        int action_point_cost=s->jobitems[id].action_point_cost;   //减少行动点
        int exp=s->jobitems[id].job_exp;   //增加经验
        s->job_exp+=exp;
        s->my_money+=salary;
        s->now_action_point-=action_point_cost;
        voiceres->give_voice_buygood();//加入音效
        give_info("完成工作啦！",false);
    }
    change_topbar();   //更新上面的数据
    job_level();     //更新进度条


}
//进度条显示数据函数
int base_window::job_exp(int exp){

    int total_exp = s->job_level*8+14;
    int results=0;
    if(exp>=total_exp){
        s->job_level+=1;
        results=double(exp-total_exp)/double(s->job_level*8+14)*100;
        s->job_exp-=total_exp;
        qDebug()<<results;
        return results;
    }
    else{
        results=double(exp)/double(total_exp)*100;
        qDebug()<<results;
        return results;
    }

}

