#include "bankfinal.h"

Bankfinal::Bankfinal()
{

}

Bankfinal::Bankfinal(int id, QString name, int price, int day)
{
    this->id = id;
    this->name = name;
    this->price = price;
    this->day = day;
}
