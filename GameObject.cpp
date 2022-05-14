#include "GameObject.h"

GameObject::GameObject() : curPos(0,0), direction((char)Keys::STAY)//, currStep(0)
{
	steps.clear();
}
//--------------------------------------------------------------------------------------//
GameObject::GameObject(int startPosX, int startPosY, char direction) : curPos(startPosX, startPosY), direction(direction)//, currStep(0)
{
	steps.clear();
}
//--------------------------------------------------------------------------------------//
Coord& GameObject::getCurPos() // returns 'curPos'
{
	return this->curPos;
}
//--------------------------------------------------------------------------------------//
char GameObject::getDirection() const // returns 'direction'
{
	return this->direction;
}
//--------------------------------------------------------------------------------------//
string GameObject::getSteps() const // retrun the string 'Steps'
{
	return this->steps;
}
//--------------------------------------------------------------------------------------//
void GameObject::setCurPos(Coord& pos) // sets a new value to 'curPos'
{
	this->curPos.setX(pos.getX());
	this->curPos.setY(pos.getY());
}
//--------------------------------------------------------------------------------------//
void GameObject::setCurPosY(int y)  // sets a new value to 'curPos' 'y' value
{
	this->curPos.setY(y);
}
//--------------------------------------------------------------------------------------//
void GameObject::setCurPosX(int x)  // sets a new value to 'curPos' 'x' value
{
	this->curPos.setX(x);
}
//--------------------------------------------------------------------------------------//
void GameObject::setDirection(char direction)  // sets a new value to 'direction'
{
	switch (direction)
	{
	case 'w':
		direction = (char)Keys::UP;
		break;
	case 'd':
		direction = (char)Keys::RIGHT;
		break;
	case 'x':
		direction = (char)Keys::DOWN;
		break;
	case 'a':
		direction = (char)Keys::LEFT;
		break;
	case 's':
		direction = (char)Keys::STAY;
	}

	if (direction == (char)Keys::UP || direction == (char)Keys::RIGHT || direction == (char)Keys::DOWN || direction == (char)Keys::LEFT || direction == (char)Keys::STAY)
	{
		this->direction = direction;
	}
}
//--------------------------------------------------------------------------------------//
void GameObject::addMove(char move) // add the move to 'steps' string
{
	steps.push_back(move);
}
//--------------------------------------------------------------------------------------//
char GameObject::nextMove() // pop the next move from 'steps' string
{
	char next = EMPTY_STR;

	if (steps.empty())
	{
		return next;
	}

	next= steps.back();
	steps.pop_back();
	return next;
}
//--------------------------------------------------------------------------------------//
bool GameObject::isValidDir(char direction) // check the validation of the direction
{
	return(direction == (char)Keys::UP || direction == (char)Keys::RIGHT || direction == (char)Keys::DOWN || direction == (char)Keys::LEFT || direction == (char)Keys::STAY);
}
//--------------------------------------------------------------------------------------//
int GameObject::randomBetween(int start, int end) // randomly picks a number between 'start' and 'end'
{
	return rand() % (end - start) + start;
}
//--------------------------------------------------------------------------------------//
char GameObject::randomDirection() // randomly picks a direction
{
	int randNum;

	randNum = rand() % 4;

	while (dircArray[randNum] == this->direction)
	{
		randNum = rand() % 4;
	}

	return dircArray[randNum];
}
//--------------------------------------------------------------------------------------//
void GameObject::addMoves(ifstream& file, bool& isOpened) // fill the string 'steps' with data from the file
{
	char currCh;
	currCh = file.get();

	while (currCh != END_LINE )
	{
		if (!isValidDir(currCh) || file.eof())
		{
			isOpened = false;
			file.close();
			throw "input error";
		}
		addMove(currCh);
		currCh = file.get();
	}
	reverse(steps.begin(), steps.end());
}
//--------------------------------------------------------------------------------------//
void GameObject::clearSteps() // clear the 'steps' string
{
	this->steps.clear();
}
//--------------------------------------------------------------------------------------//