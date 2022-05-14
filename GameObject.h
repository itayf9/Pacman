#pragma once

#include "Coord.h"
#include "Utilities.h"
#include "Board.h"
#include "Pacman_Utillities.h"

const char dircArray[] = { 'W','D','X','A' };

class GameObject
{
protected:
	Coord curPos;
	char direction;
	string steps;

public:
//--------------------------------------------------------------------------------------//
	GameObject();
	GameObject(int startPosX, int startPosY, char direction);
//--------------------------------------------------------------------------------------//
	Coord& getCurPos(); // returns 'curPos'
	char   getDirection() const; // returns 'direction'
	string getSteps() const; // retrun the string 'Steps'
//--------------------------------------------------------------------------------------//
	void setCurPos(Coord& pos); // sets a new value to 'curPos'
	void setCurPosY(int y); // sets a new value to 'curPos' 'y' value
	void setCurPosX(int x); // sets a new value to 'curPos' 'x' value
	void setDirection(char direction); // sets a new value to 'direction'
//--------------------------------------------------------------------------------------//
	bool isValidDir(char direction); // check the validation of the direction
	void addMove(char move); // add the move to 'steps' string
	char nextMove(); // pop the next move from 'steps' string
	int randomBetween(int start, int end); // randomly picks a number between 'start' and 'end'
	char randomDirection(); // randomly picks a direction
	void addMoves( ifstream& file, bool& isOpened); // fill the string 'steps' with data from the file
	void clearSteps(); // clear the 'steps' string
};

