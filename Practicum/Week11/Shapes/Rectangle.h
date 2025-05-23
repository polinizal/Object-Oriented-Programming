#pragma once

#include "Shape.h"

class Rectangle : public Shape
{
public:
	Rectangle(int x1, int y1, int x2, int y2);
	Rectangle(const Point& p1, const Point& p2);

	double getArea() const override;
	bool isPointInside(int x, int y) const override;

	Shape* clone() const override;

	Intersection intersect(const Shape* shape) const override;

	Intersection intersectWith(const Triangle* shape) const override;
	Intersection intersectWith(const Rectangle* shape) const override;
	Intersection intersectWith(const Circle* shape) const override;
};