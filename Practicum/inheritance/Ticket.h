#pragma once
#include "String.h"

class Ticket
{
protected:
	 String name;
	 double price;

public:
	 Ticket(const String name,double price)
	 {
		 setName(name);
		 setPrice(price);
	 }

	 Ticket()
	 {

	 }

	 void setName(const String &name);
	 void setPrice(double price);

	 String getName() const;
	 double getPrice() const;

	 void PrintNormal() {

	 }

	 void printInfo() const {
		 std::cout <<"Ticket type: Normal  " << "Play: " << name << ", Price: " << price << " BGN" << std::endl;
	 }
};

