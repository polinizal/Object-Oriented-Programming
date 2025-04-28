#pragma once
#include "Ticket.h"
#include "String.h"

class GroupTicket : Ticket
{
	GroupTicket(const String &name, double price)
	{
		setName(name);
		setPrice(price);
	}

	void setName(const String &name);
	void setPrice(double price);
	
	void printInfo() const {
		std::cout << "Ticket type: Group  " << "Play: " << name << ", Price: " << this->price << " BGN" << std::endl;
	}
};

