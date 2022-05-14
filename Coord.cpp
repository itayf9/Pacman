#include "Coord.h"

Coord::Coord()
{
	this->x = 0;
	this->y = 0;
}
//--------------------------------------------------------------------------------------//
Coord::Coord(int x, int y) : x(x), y(y)
{
}
//--------------------------------------------------------------------------------------//
Coord::Coord(const Coord& other)
{
	*this = other;
}
//--------------------------------------------------------------------------------------//
bool Coord::operator==(const Coord& x) const
{
	if (x.getX() == this->getX() && x.getY() == this->getY())
	{
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------//
Coord& Coord::operator=(const Coord& e)
{
	this->x = e.x;
	this->y = e.y;
	return *this;
}
//--------------------------------------------------------------------------------------//
int Coord::getX() const // returns 'x'
{
	return x;
}
//--------------------------------------------------------------------------------------//
int Coord::getY() const // returns 'y'
{
	return y;
}
//--------------------------------------------------------------------------------------//
void Coord::setX(int x) // sets a new value to 'x'
{
	this->x = x;
}
//--------------------------------------------------------------------------------------//
void Coord::setY(int y) // sets a new value to 'y'
{
	this->y = y;
}
//--------------------------------------------------------------------------------------//