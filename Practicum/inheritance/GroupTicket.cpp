#include "GroupTicket.h"


void GroupTicket::setName(const String &name)
{
	this->name = name;
}

void GroupTicket::setPrice(double price)
{
	this->price = 0.8 * price;
}
