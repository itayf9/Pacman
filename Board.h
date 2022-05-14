#pragma once

#include "Utilities.h"
#include "Pacman_Utillities.h"

const int MAX_COLS = 80;
const int MAX_ROWS = 25;

enum class Board_Symbols
{
	FRAME = 178,
	WALL = (char)219,
	BREAD = (char)250,
	HEART = (char)003,
	SPACE = ' ',
};

class Board
{
private:
	char    board[MAX_ROWS][MAX_COLS];
	int hight;
	int width;
	int countBread;
	Coord legendPos;

public:
	Board();
//--------------------------------------------------------------------------------------//
	int getHight() const; // returns 'hight'
	int getWidth() const; // returns 'width'
	Coord& getLegendPos(); // returns 'legendPos'
	char getCell(int y, int x) const; // returns 'board[y][x]'
	int getCountBread()const; // returns 'countBread'
//--------------------------------------------------------------------------------------//
	void setHight(int hight); // sets a new value to 'hight'
	void setWidth(int width); // sets a new value to 'width'
	void setLegendPos(const Coord& legendPos); // sets a new value to 'legendPos'
	void setCell(int y, int x, char ch) ; // sets a new value to 'board[y][x]'
	void setCountBread(int count); // sets a new value to 'countBread'
//--------------------------------------------------------------------------------------//
	void decreaseCountBread(); // decreases the 'countBread' value by one
	void fixLegendArea(); // corrects the legend area in the board
	void resetBoard(); // resets the board parameters
};
