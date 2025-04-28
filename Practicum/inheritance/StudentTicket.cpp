#include "StudentTicket.h"
#include "Ticket.h"


void StudentTicket::setName(const String &name)
{
	if (name == nullptr) {
		return;
	}
	this->name = name;
}

void StudentTicket::setPrice(double price)
{
	Ticket::setPrice(price / 2);
}
