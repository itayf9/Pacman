#include "Board.h"

Board::Board() :hight(MAX_ROWS), width(MAX_COLS), countBread(INT_ZERO), legendPos()
{
	resetBoard();
}
//--------------------------------------------------------------------------------------//
int Board::getHight() const // returns 'hight'
{
	return this->hight;
}
//--------------------------------------------------------------------------------------//
int Board::getWidth() const // returns 'width'
{
	return this->width;
}
//--------------------------------------------------------------------------------------//
Coord& Board::getLegendPos() // returns 'legendPos'
{
	return this->legendPos;
}
//--------------------------------------------------------------------------------------//
void Board::setHight(int hight) // sets a new value to 'hight'
{
	this->hight = hight;
}
//--------------------------------------------------------------------------------------//
void Board::setWidth(int width) // sets a new value to 'width'
{
	this->width = width;
}
//--------------------------------------------------------------------------------------//
void Board::setLegendPos(const Coord& legendPos) // sets a new value to 'legendPos'
{
	this->legendPos = legendPos;
}
//--------------------------------------------------------------------------------------//
char Board::getCell(int y, int x) const // returns 'board[y][x]'
{
	return this->board[y][x];
}
//--------------------------------------------------------------------------------------//
int Board::getCountBread() const // returns 'countBread'
{
	return this->countBread;
}
//--------------------------------------------------------------------------------------//
void Board::decreaseCountBread() // decreases the 'countBread' value by one
{
	this->countBread--;
}
//--------------------------------------------------------------------------------------//
void Board::fixLegendArea() // corrects the legend area in the board
{
	int legendX = getLegendPos().getX();
	int legendY = getLegendPos().getY();
	for (int k = 0; k < 20; k++)
	{
		if (legendX + k < MAX_COLS)
		{
			if (legendY < MAX_ROWS)
			{
				if (board[legendY][legendX + k] == (char)Board_Symbols::BREAD)
				{
					countBread--;
				}
				setCell(legendY, legendX + k, (char)File_Symbols::LIMITS);
			}
			if (legendY + 1 < MAX_ROWS)
			{
				if (board[legendY + 1][legendX + k] == (char)Board_Symbols::BREAD)
				{
					countBread--;
				}
				setCell(legendY + 1, legendX + k, (char)File_Symbols::LIMITS);
			}
			if (legendY + 2 < MAX_ROWS)
			{
				if (board[legendY + 2][legendX + k] == (char)Board_Symbols::BREAD)
				{
					countBread--;
				}
				setCell(legendY + 2, legendX + k, (char)File_Symbols::LIMITS);
			}
		}
	}
}
//--------------------------------------------------------------------------------------//
void Board::resetBoard() // resets the board parameters
{
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			this->board[i][j] = '\0';
		}
	}
}
//--------------------------------------------------------------------------------------//
void Board::setCell(int y, int x, char ch)  // sets a new value to 'board[y][x]'
{
	board[y][x] = ch;
}
//--------------------------------------------------------------------------------------//
void Board::setCountBread(int count) // sets a new value to 'countBread'
{
	this->countBread = count;
}
//--------------------------------------------------------------------------------------//
