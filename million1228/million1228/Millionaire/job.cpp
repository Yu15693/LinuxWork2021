#include "job.h"

job::job()
{

}


job::job(int id, QString job_name, int job_exp,int action_point_cost, int job_price,int job_level){
    this->id=id;
    this->job_name=job_name;
    this->action_point_cost = action_point_cost;
    this->job_exp=job_exp;
    this->job_level=job_level;
    this->job_price = job_price;
}
