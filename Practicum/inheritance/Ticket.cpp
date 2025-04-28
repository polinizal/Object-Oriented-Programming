#include "Ticket.h"

void Ticket::setName(const String &name)
{
	if (name == nullptr) {
		return;
	}
	this->name = name;
}

void Ticket::setPrice(double price)
{
	if (price < 0 || price>1000) {
		return;
	}
	this->price = price;
}

String Ticket::getName() const
{
	return this->name;
}

double Ticket::getPrice() const
{
	return this->price;
}
