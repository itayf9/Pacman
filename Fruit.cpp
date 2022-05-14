#include "Fruit.h"

Fruit::Fruit() : GameObject(), fruitVal(randomBetween(5, 9)), showTime(0), hideTime(0), isAppear(false)
{
	countMoveStraight = randomBetween(5,15);
	hideAndShow.clear();
	positions.clear();
}
//--------------------------------------------------------------------------------------//
char Fruit::getFruitVal() const // returns 'fruitVal'
{
	return this->fruitVal;
}
//--------------------------------------------------------------------------------------//
int Fruit::getShowTime() const // returns 'showTime'
{
	return this->showTime;
}
//--------------------------------------------------------------------------------------//
void Fruit::setFruitVal(char fruitVal) // sets a new value to 'fruitVal'
{
	this->fruitVal = fruitVal;
}
//--------------------------------------------------------------------------------------//
int Fruit::getHideTime() const // returns 'hideTime'
{
	return this->hideTime;
}
//--------------------------------------------------------------------------------------//
bool Fruit::getIsAppear() const // returns 'isAppear'
{
	return this->isAppear;
}
//--------------------------------------------------------------------------------------//
vector<int>& Fruit::getHideAndShowVec() // returns Hide And Show Vector
{
	return this->hideAndShow;
}
//--------------------------------------------------------------------------------------//
vector<Coord>& Fruit::getPositionsVec() // returns Positions Vector
{
	return this->positions;
}
//--------------------------------------------------------------------------------------//
string& Fruit::getFruitValues() // returns 'fruitValues'
{
	return this->fruitValues;
}
//--------------------------------------------------------------------------------------//
void Fruit::setShowTime(int showTime) // sets a new value to 'showTime'
{
	this->showTime = showTime;
}
//--------------------------------------------------------------------------------------//
void Fruit::setHideTime(int hideTime) // sets a new value to 'hideTime'
{
	this->hideTime = hideTime;
}
//--------------------------------------------------------------------------------------//
void Fruit::decreaseHideTime() // decreases the 'hideTime' value by one
{
	this->hideTime--;
}
//--------------------------------------------------------------------------------------//
void Fruit::fruitMove(const Board& board,int gameMode) // handles the fruit movement
{
	bool toAdd = true;

	if (gameMode == (int)Game_Mode::LOAD && this->countMoveStraight == 0)
	{
		this->countMoveStraight++;
	}

	if (this->countMoveStraight == 0)
	{
		this->direction = randomDirection();
		this->countMoveStraight = randomBetween(5, 15);
	}

	switch (this->direction)// handle fruit movement
	{
	case (char)Keys::UP:
		if (board.getCell(this->curPos.getY() -1, this->curPos.getX()) == (char)Board_Symbols::WALL || this->curPos.getY() - 1 == 0)
		{
			this->direction = randomDirection();
			toAdd = false;
		}
		else
		{
			this->curPos.setY(this->curPos.getY() - 1);
			this->countMoveStraight-- ;
		}
		break;
	case (char)Keys::RIGHT:
		if (board.getCell(this->curPos.getY() , this->curPos.getX()+1) == (char)Board_Symbols::WALL || this->curPos.getX() + 1 == board.getWidth() - 1)
		{
			this->direction = randomDirection();
			toAdd = false;
		}
		else
		{
			this->curPos.setX(this->curPos.getX() + 1);
			this->countMoveStraight-- ;
		}
		break;
	case (char)Keys::DOWN:
		if (board.getCell(this->curPos.getY()+1, this->curPos.getX()) == (char)Board_Symbols::WALL || this->curPos.getY() + 1 == board.getHight() - 1)
		{
			this->direction = randomDirection();
			toAdd = false;
		}
		else
		{
			this->curPos.setY(this->curPos.getY() + 1);
			this->countMoveStraight--;
		}
		break;
	case (char)Keys::LEFT:
		if (board.getCell(this->curPos.getY(), this->curPos.getX() - 1) == (char)Board_Symbols::WALL || this->curPos.getX() - 1 == 0)
		{
			this->direction = randomDirection();
			toAdd = false;
		}
		else
		{
			this->curPos.setX(this->curPos.getX() - 1);
			this->countMoveStraight--;
		}
		break;
	case (char)Keys::STAY:
		break;
		
	}
	if (gameMode == (int)Game_Mode::SAVE)
	{
		if (toAdd)
		{
			this->addMove(this->getDirection());
		}
		else
		{
			this->addMove((char)Keys::STAY);
		}
	}
}
//--------------------------------------------------------------------------------------//
void Fruit::addHideAndShow(int hs) // add new time to Hide And Show Vector
{
	hideAndShow.push_back(hs);
}
//--------------------------------------------------------------------------------------//
void Fruit::addPositions(const Coord& pos) // add new position to positions Vector
{
	positions.push_back(pos);
}
//--------------------------------------------------------------------------------------//
void Fruit::addFruitValues(ifstream& file, bool& isOpened) // fill the string 'fruitValues' with data from the file
{
	char currCh;
	currCh = file.get();

	while (currCh != END_LINE)
	{
		if (currCh < '5' || currCh > '9' || file.eof())
		{
			isOpened = false;
			file.close();
			throw "input error";
		}
		addFruitValue(currCh);
		currCh = file.get();
	}
	reverse(fruitValues.begin(), fruitValues.end());
}
//--------------------------------------------------------------------------------------//
int Fruit::nextTime() // pop the next time from the vector
{
	int next = END_OF_INT_VEC;

	if (hideAndShow.empty())
	{
		return next;
	}

	next = hideAndShow.back();
	hideAndShow.pop_back();
	return next;
}
//--------------------------------------------------------------------------------------//
Coord Fruit::nextPos() // pop the next position from the vector
{
	Coord next(MAX_COLS, MAX_ROWS);

	if (positions.empty())
	{
		return next;
	}

	next = positions.back();
	positions.pop_back();
	return next;
}
//--------------------------------------------------------------------------------------//
char Fruit::nextVal() // pop the next value from the vector
{
	char next = EMPTY_STR;

	if (fruitValues.empty())
	{
		return next;
	}

	next = fruitValues.back();
	fruitValues.pop_back();
	return next;
}
//--------------------------------------------------------------------------------------//
void Fruit::addFruitValue(char val) // add new value to 'fruitValues' Vector
{
	fruitValues.push_back(val);
}
//--------------------------------------------------------------------------------------//
void Fruit::resetFruit(int& fruitIteration) // resets the fruit's parameters, in load mode
{
	setFruitVal(CHAR_ZERO);
	fruitIteration = nextTime() + 1;
	isAppear = false;
}
//--------------------------------------------------------------------------------------//
void Fruit::resetFruit(bool& isFruitFound,int& fruitIteration) // resets the fruit's parameters
{
	setFruitVal(CHAR_ZERO);
	setShowTime(INT_ZERO);
	setHideTime(randomBetween(10, 120));
	isFruitFound = false;
}
//--------------------------------------------------------------------------------------//
void Fruit::decreaseShowTime() // decreases the 'showTime' value by one
{
	this->showTime--;
}
//--------------------------------------------------------------------------------------//
void Fruit::setIsAppear(bool isAppear) // sets a new value to 'isAppear'
{
	this->isAppear = isAppear;
}
//--------------------------------------------------------------------------------------//

