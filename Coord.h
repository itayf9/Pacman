#pragma once

class Coord
{
private:
	int x;
	int y;

public:
	//--constructors------------------------------------------------------------------------//
	Coord();
	Coord(int x, int y);
	Coord(const Coord& other);

	bool operator== (const Coord& x) const;
	Coord& operator= (const Coord& e);
	//--getter------------------------------------------------------------------------------//
	int getX() const; // returns 'x'
	int getY() const; // returns 'y'
//--setter------------------------------------------------------------------------------//
	void setX(int x); // sets a new value to 'x'
	void setY(int y); // sets a new value to 'y'
	bool samePlace(Coord& coord) { return *this == coord; } // checks if two coord are equal (using the operator '==')
};