#ifndef JOB_H
#define JOB_H

#include <QObject>

class job
{
public:

    int id;//工作id
    QString job_name;//工作名称
    int action_point_cost;//消耗的行动点数
    int job_time;    //工作时间
    int job_price;   //工资  每天多少钱
    int job_exp;    //工作可获得经验
    int job_level;   //完成工作所需等级


    job();
    job(int id, QString job_name,int job_exp, int action_point_cost, int job_price,int job_level);


};

#endif // JOB_H
