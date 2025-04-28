#pragma once
#include "String.h"
#include "Ticket.h"
class StudentTicket : public Ticket
{
public:
	StudentTicket(const String &name,const double price)
	{
		setName(name);
		setPrice(price);
	}


	void setName(const String &name);
	void setPrice(double price);

	void printInfo() const {
		std::cout << "Ticket type: Student  " << "Play: " << name << ", Price: " << this->price << " BGN" << std::endl;
	}
};

