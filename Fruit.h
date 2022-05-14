#pragma once

#include "GameObject.h"

class Fruit : public GameObject
{
private:
	char fruitVal;
	int showTime;
	int hideTime;
	bool isAppear;
	int countMoveStraight;
	vector<int> hideAndShow;
	vector<Coord> positions;
	string fruitValues;

public:
	Fruit();
//--------------------------------------------------------------------------------------//
	char getFruitVal() const; // returns 'fruitVal'
	int getShowTime() const; // returns 'showTime'
	int getHideTime() const; // returns 'hideTime'
	bool getIsAppear() const; // returns 'isAppear'
	vector<int>& getHideAndShowVec(); // returns Hide And Show Vector
	vector<Coord>& getPositionsVec(); // returns Positions Vector
	string& getFruitValues(); // returns 'fruitValues'
//--------------------------------------------------------------------------------------//
	void setFruitVal(char fruitVal); // sets a new value to 'fruitVal'
	void setShowTime(int showTime); // sets a new value to 'showTime'
	void setHideTime(int hideTime); // sets a new value to 'hideTime'
	void setIsAppear(bool isAppear); // sets a new value to 'isAppear'
//--------------------------------------------------------------------------------------//
	void decreaseHideTime(); // decreases the 'hideTime' value by one
	void decreaseShowTime(); // decreases the 'showTime' value by one

	void addHideAndShow(int hs); // add new time to Hide And Show Vector
	void addPositions(const Coord& pos); // add new position to positions Vector
	void addFruitValue(char val); // add new value to 'fruitValues' Vector
	void addFruitValues(ifstream& file, bool& isOpened); // fill the string 'fruitValues' with data from the file
	int nextTime(); // pop the next time from the vector
	Coord nextPos(); // pop the next position from the vector
	char nextVal(); // pop the next value from the vector
//--------------------------------------------------------------------------------------//
	void fruitMove(const Board& board,int gameMode); // handles the fruit movement
	void resetFruit(int& fruitIteration); // resets the fruit's parameters, in load mode
	void resetFruit(bool& isFruitFound, int& fruitIteration); // resets the fruit's parameters
};

